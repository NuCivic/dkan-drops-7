<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class EntityTest.
 */
class EntityTest extends AutoloadTestBase {

  /**
   * {@inheritdoc}
   */
  protected static $modules = array('autoload_test_entity_ui');

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return static::info(t('Ensure autoloading is properly operable in entity hooks.'));
  }

  /**
   * {@inheritdoc}
   */
  public function test() {
    // The next error must not appear:
    // Error: Class 'Drupal\autoload_test_entity_ui\ViewsController' not found
    // in entity_views_data() (line 31 of sites/all/modules/entity/views/entity.
    // views.inc).
    $entity_info = entity_get_info('autoload_test_entity');

    $this->assertTrue(
      class_exists($entity_info['views controller class']),
      'Views controller class for entity loaded successfully.'
    );
  }

}
