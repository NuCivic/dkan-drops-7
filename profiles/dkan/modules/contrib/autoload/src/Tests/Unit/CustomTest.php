<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class CustomTest.
 */
class CustomTest extends AutoloadTestBase {

  /**
   * {@inheritdoc}
   */
  protected static $modules = array('autoload_test_custom');

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return static::info(t('Testing functionality of autoloading custom namespaces.'));
  }

  /**
   * {@inheritdoc}
   */
  public function test() {
    // If something will not work we'll get fatal error :)
    new \Autoload\Tests\PSR0();
    new \Autoload\Tests\PSR4();
    new \AutoloadTests\PSR4();
    new \Autoload\Tests\Example\Test();

    $autoload = autoload();

    // Initially namespace defined incorrectly for this class and it must not
    // be available.
    $this->assertFalse(
      isset($autoload['AutoloadWrongNamespace\WrongNamespace']),
      'A correct class does not exist in the autoloading map due to a wrong autoloading declaration.'
    );
    $this->assertFalse(
      class_exists('AutoloadWrongNamespace\WrongNamespace'),
      'A class cannot be loaded because of wrongly defined autoloading.'
    );

    // That's how namespace will be stored in a mapping.
    $this->assertTrue(
      isset($autoload['Autoload\WrongNamespace\WrongNamespace']),
      'A non-existent class within the autoloading map because autoloading was wrongly declared.'
    );
    $this->assertFalse(
      class_exists('Autoload\WrongNamespace\WrongNamespace'),
      'Cannot load non-existent class even if it is defined in a class map.'
    );

    // Despite on wrong namespace path, the path to file is correct and it'll
    // be included by autoloader after an attempt to load the wrong namespace.
    // This will allow a correct namespace to work.
    $this->assertTrue(
      class_exists('AutoloadWrongNamespace\WrongNamespace'),
      'A correct class became available because of including the file during appealing to non-existent class.'
    );
  }

}
