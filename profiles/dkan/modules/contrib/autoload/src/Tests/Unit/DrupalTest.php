<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class DrupalTest.
 */
class DrupalTest extends AutoloadTestBase {

  /**
   * {@inheritdoc}
   */
  protected static $modules = array('autoload_test_drupal');

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return static::info(t('Testing functionality of autoloading the Drupal-way namespaces.'));
  }

  /**
   * {@inheritdoc}
   */
  public function test() {
    // If something will not work we'll get fatal error :)
    new \Drupal\autoload_test_drupal\PSR0();
    new \Drupal\autoload_test_drupal\PSR4();
  }

}
