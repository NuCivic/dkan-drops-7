<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class UnitTest.
 */
abstract class AutoloadTestBase extends \DrupalWebTestCase {

  /**
   * Modules which should enabled for testing.
   *
   * @var string[]
   */
  protected static $modules = array('autoload');
  /**
   * {@inheritdoc}
   */
  protected $profile = 'minimal';
  /**
   * A path to file with the autoloading class map.
   *
   * @var string
   */
  protected $autoloadFile = '';

  /**
   * Returns an information about the test.
   *
   * @param string $description
   *   Test description.
   *
   * @return array
   *   An information about the test.
   */
  protected static function info($description) {
    $parts = explode('\\', get_called_class());

    return array(
      // Use class name as name of the test.
      'name' => end($parts),
      // The "1" will always contain the name of module.
      'group' => $parts[1],
      'description' => $description,
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    $this->autoloadFile = drupal_get_path('module', 'autoload') . '/tests/autoload.php';

    parent::setUp(static::$modules);

    $this->assertTrue(
      variable_get('autoload_file') === $this->autoloadFile,
      sprintf('The autoloading class map will be used from the "%s" file.', $this->autoloadFile)
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function preloadRegistry() {
    parent::preloadRegistry();

    // Tests, possibly, doesn't have a writing permissions to a filesystem.
    // So, let's just use actual generated file.
    //
    // WE MUST set this variable here and in this way because "setUp()" runs
    // "resetAll()" method which triggers Drupal cache clearing during which
    // a lot of modules start collecting the data to fill the cache (e.g.
    // "entity", "views" etc.). In a case of using autoloading in one of the
    // hooks providing the data, we'll get a fatal error not doing this.
    /* @see entity_views_data() */
    variable_set('autoload_file', $this->autoloadFile);
  }

}
