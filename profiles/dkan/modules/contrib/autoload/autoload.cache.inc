<?php

/**
 * @file
 * Autoload cache controller.
 */

/**
 * An object, behaving as an array.
 */
class ArrayContainer implements \ArrayAccess, \Iterator, \Countable {

  /**
   * Data storage.
   *
   * @var array[]
   */
  protected $data = array();

  /**
   * {@inheritdoc}
   */
  public function __construct(array $data = array()) {
    $this->data = $data;
  }

  /**
   * {@inheritdoc}
   */
  public function offsetExists($offset) {
    return isset($this->data[$offset]);
  }

  /**
   * {@inheritdoc}
   */
  public function offsetGet($offset) {
    return $this->data[$offset];
  }

  /**
   * {@inheritdoc}
   */
  public function offsetSet($offset, $value) {
    $this->data[$offset] = $value;
  }

  /**
   * {@inheritdoc}
   */
  public function offsetUnset($offset) {
    unset($this->data[$offset]);
  }

  /**
   * {@inheritdoc}
   */
  public function current() {
    return current($this->data);
  }

  /**
   * {@inheritdoc}
   */
  public function next() {
    next($this->data);
  }

  /**
   * {@inheritdoc}
   */
  public function key() {
    return key($this->data);
  }

  /**
   * {@inheritdoc}
   */
  public function valid() {
    return $this->key() !== NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function rewind() {
    reset($this->data);
  }

  /**
   * {@inheritdoc}
   */
  public function count() {
    return count($this->data);
  }

}

/**
 * Class AutoloadCache.
 */
class AutoloadCache extends ArrayContainer {

  /**
   * Name of the table in database to store the data when IO is not available.
   */
  const BIN = 'cache_autoload';

  /**
   * A path to file for storing the autoloading map.
   *
   * @var string
   */
  protected $file = '';

  /**
   * {@inheritdoc}
   *
   * @param string $file
   *   A path to file for storing the autoloading map.
   */
  public function __construct($file) {
    parent::__construct();

    $this->file = $file;

    if (is_readable($this->file)) {
      $this->data = require $this->file;
    }

    // If the file doesn't exist, not readable or doesn't contain a map.
    if (empty($this->data) || !is_array($this->data)) {
      $this->data = db_select(static::BIN)
        ->fields(static::BIN)
        ->execute()
        ->fetchAllAssoc('namespace', \PDO::FETCH_ASSOC);

      // Rebuild if a database is empty, try to store data in file otherwise.
      empty($this->data) ? $this->rebuild() : $this->updateFile();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function offsetSet($namespace, $value) {
    if (
      !is_array($value) ||
      empty($value['file']) ||
      empty($value['provider']) ||
      !file_exists($value['file'])
    ) {
      throw new \InvalidArgumentException(sprintf('Incorrect value for the "%s()" method.', __METHOD__));
    }

    parent::offsetSet($namespace, $value);
  }

  /**
   * Rebuild the autoloading map.
   */
  public function rebuild() {
    // Hack, allowing bringing to life a site with a broken registry.
    if (!function_exists('system_rebuild_module_data')) {
      drupal_load('module', 'system');
    }

    $mask = sprintf(
      '/^' . DRUPAL_PHP_FUNCTION_PATTERN . '(%s)$/',
      implode('|', array_map('preg_quote', autoload_extensions()))
    );

    // Ensure map will be newly constructed.
    $this->data = array();

    foreach (system_rebuild_module_data() as $module_name => $data) {
      if (empty($data->info['autoload'])) {
        continue;
      }

      $module_path = pathinfo($data->filename, PATHINFO_DIRNAME);

      // Allow "autoload = whatever" to enable Drupal-way namespaces.
      if (!is_array($data->info['autoload'])) {
        $data->info['autoload'] = array();
      }

      /* @see simpletest_test_get_all() */
      $data->info['autoload'] = array_merge_recursive($data->info['autoload'], array_fill_keys(
        array("lib/Drupal/$module_name", 'src'),
        array("Drupal\\$module_name")
      ));

      foreach ($data->info['autoload'] as $subdirectory => $namespace_prefixes) {
        // Handle "autoload[] = NS" instead of "autoload[dir][] = NS".
        if (!is_array($namespace_prefixes)) {
          continue;
        }

        // Module path will always be without slashes but subdirectory can be
        // with both because this value is user-related.
        $sources_path = $module_path . '/' . trim($subdirectory, '/');

        foreach (file_scan_directory($sources_path, $mask) as $pathinfo) {
          // "array_filter()" - to remove emptinesses caused by slashes.
          $relative_path = array_filter(explode('/', str_replace($sources_path, '', dirname($pathinfo->uri))));
          $relative_path[] = $pathinfo->name;

          foreach ($namespace_prefixes as $namespace_prefix) {
            $namespace_prefix = explode('\\', $namespace_prefix);
            $_relative_path = $relative_path;

            // Count without removing slashes because PSR-4 namespace can
            // consist only of a single part with trailing backslash at the
            // end. For example: "IamCorrectPsr4\".
            if (count($namespace_prefix) === 1) {
              // Use "reset()" instead of "[0]" for every operands in
              // condition because "array_filter()" will change enumeration
              // of keys in case of removing elements.
              if (reset($namespace_prefix) !== reset($_relative_path)) {
                trigger_error(sprintf('Incorrectly defined autoloading for the "%s" file provided by the "%s" module.', $pathinfo->uri, $module_name), E_USER_WARNING);
                continue;
              }

              array_shift($_relative_path);
            }

            // Do not use the "offsetSet()" method here to omit redundant logic
            // in there.
            $this->data[implode('\\', array_merge(array_filter($namespace_prefix), $_relative_path))] = array(
              'file' => $pathinfo->uri,
              'provider' => $module_name,
            );
          }
        }
      }
    }

    drupal_alter('autoload_lookup', $this);

    // A file cannot be written so store the map to the database.
    if (!$this->updateFile()) {
      db_truncate(static::BIN)->execute();

      foreach ($this->data as $namespace => $data) {
        db_insert(static::BIN)
          ->fields(array('namespace' => $namespace) + $data)
          ->execute();
      }

      trigger_error(sprintf('Autoloading map saved to the database because the "%s" file is not writable. You are allowed to modify it during development only and never any more.', $this->file), E_USER_WARNING);
    }
  }

  /**
   * Dumps the autoloading class map to the file.
   *
   * @return bool
   *   A status whether the file has been saved.
   */
  protected function updateFile() {
    $reference = __METHOD__;
    $map = static::exportVariable($this->data);
    $data = <<<PHP
<?php

/**
 * @file
 * Auto-generated autoloading class map.
 *
 * @see $reference()
 */

return $map;

PHP;

    return FALSE !== file_unmanaged_save_data($data, $this->file, FILE_EXISTS_REPLACE);
  }

  /**
   * Export a variable as a well-formatted (in terms of Drupal CS) string.
   *
   * @param mixed $var
   *   Variable to dump into a string.
   * @param int $prefix
   *   A number of spaces to put before the code.
   * @param bool $init
   *   Internal variable to handle recursion.
   *
   * @return string
   *   Dumped variable.
   */
  protected static function exportVariable($var, $prefix = 0, $init = TRUE) {
    if (is_array($var)) {
      if (empty($var)) {
        $output = 'array()';
      }
      else {
        $output = "array(\n";
        foreach ($var as $key => $value) {
          // Using normal "var_export()" on the key to ensure correct quoting.
          $output .= '  ' . var_export($key, TRUE) . ' => ' . call_user_func(__METHOD__, $value, 2, FALSE) . ",\n";
        }
        $output .= ')';
      }
    }
    elseif (is_bool($var)) {
      $output = $var ? 'TRUE' : 'FALSE';
    }
    elseif (is_int($var)) {
      $output = intval($var);
    }
    elseif (is_numeric($var)) {
      $floatval = floatval($var);

      if (is_string($var) && (string) $floatval !== $var) {
        // Do not convert a string to a number if the string
        // representation of that number is not identical to the
        // original value.
        $output = var_export($var, TRUE);
      }
      else {
        $output = $floatval;
      }
    }
    elseif (is_string($var) && strpos($var, "\n") !== FALSE) {
      // Replace line breaks in strings with a token for replacement
      // at the very end. This protects whitespace in strings from
      // unintentional indentation.
      $output = var_export(str_replace("\n", '***BREAK***', $var), TRUE);
    }
    else {
      $output = var_export($var, TRUE);
    }

    if ($prefix > 0) {
      $output = str_replace("\n", "\n" . str_repeat(' ', $prefix), $output);
    }

    if ($init) {
      $output = str_replace('***BREAK***', "\n", $output);
    }

    return $output;
  }

  /**
   * Returns schema for the table of autoloading storage.
   *
   * @return array[]
   *   Database table schemas.
   *
   * @see autoload_schema()
   */
  public static function schema() {
    $schema = array();

    $schema['description'] = 'Stores fallback cache of autoloading mapping.';

    foreach (array(
      'file' => 'A path to file relative to the Drupal root directory.',
      'provider' => 'Name of the module providing the file.',
      'namespace' => 'Fully-qualified path to object in terms of PHP.',
    ) as $field => $description) {
      $schema['fields'][$field] = array(
        'type' => 'varchar',
        'length' => 255,
        'default' => '',
        'not null' => TRUE,
        'description' => $description,
      );
    }

    $schema['unique keys'] = array(
      'file' => array('file'),
    );

    $schema['primary key'] = array(
      'namespace',
    );

    return array(static::BIN => $schema);
  }

}
