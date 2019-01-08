<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class CacheTest.
 */
class CacheTest extends AutoloadTestBase {

  /**
   * {@inheritdoc}
   */
  protected static $modules = array('autoload_test_lookup');

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return static::info(t('Testing generation of the autoloading class map.'));
  }

  /**
   * Tests the autoloading cache.
   */
  public function test() {
    $autoload_file = drupal_realpath('private://autoload.php');
    $autoload = new \AutoloadCache($autoload_file);
    $autoload->rebuild();

    if ($this->assertTrue(is_readable($autoload_file), sprintf('The autoloading map successfully saved to the "%s" file.', $autoload_file))) {
      $map = array();

      // Check implementation of "\Iterator" interface.
      foreach ($autoload as $namespace => $data) {
        $map[$namespace] = $data;
      }

      foreach (array(
        'file' => $autoload_file,
        'data' => $map,
      ) as $property => $value) {
        $reflection = new \ReflectionProperty($autoload, $property);
        $reflection->setAccessible(TRUE);
        $this->assertTrue($reflection->getValue($autoload) === $value, sprintf('The "%s" property of the "%s" class has expected value.', $property, get_class($autoload)));
        $reflection->setAccessible(FALSE);
      }

      $dump = require $autoload_file;
      // Check implementation of "\Countable" interface.
      $count = count($autoload);

      $this->assertTrue(in_array($count, array(count($map), count($dump)), TRUE), sprintf('The autoloading class map successfully dumped %d entries.', $count));
      $this->assertTrue($map === $dump, 'The autoloading class map successfully interpreted as PHP code.');
    }

    // The class is defined within a map but does not actually available
    // since here we have a testing scenario that doesn't affect on the
    // autoloading process.
    /* @see autoload_test_lookup_autoload_lookup_alter() */
    $this->assertTrue(
      isset($autoload['ArchiverInterface']),
      'A custom definition has been successfully provided by lookup alteration.'
    );

    foreach (array(
      'bla' => array(),
      'test' => 1,
      // Correct provider but non-existent file.
      'alpha' => array(
        'file' => 'asdasd',
        'provider' => 'autoload',
      ),
    ) as $namespace => $data) {
      try {
        $autoload[$namespace] = $data;
        $this->fail('An attempt to set incorrect value ended up with a success.');
      }
      catch (\Exception $e) {
      }
    }
  }

}
