<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class ExtensionsTest.
 */
class ExtensionsTest extends AutoloadTestBase {

  /**
   * {@inheritdoc}
   */
  protected static $modules = array('autoload_test_extensions');

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return static::info(t('Ensure registered file extensions are available for autoloading.'));
  }

  /**
   * {@inheritdoc}
   */
  public function test() {
    // These extensions are always available!
    $this->assertExtensions(array('.php', '.inc'));

    // Only Drupal can change this. Modules cannot!
    spl_autoload_extensions('.test');

    // List of extensions changed as expected.
    $this->assertExtensions(array('.php', '.inc', '.test'));
    // Class must not exist since an extension was registered not by Drupal.
    $this->assertFalse(
      class_exists('Drupal\autoload_test_extensions\PSR4'),
      'A class cannot be loaded despite on registered file extension.'
    );
  }

  /**
   * Assert registered extensions for autoloading.
   *
   * @param string[] $extensions
   *   Extensions list. Each one must start from dot.
   */
  protected function assertExtensions(array $extensions) {
    $this->assertFalse(array_diff(autoload_extensions(), $extensions), 'Required extensions available.');
  }

}
