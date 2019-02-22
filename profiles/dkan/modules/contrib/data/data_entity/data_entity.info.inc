<?php

/**
 * @file
 * Provides metadata for data entities.
 */

/**
 * Implements hook_entity_property_info().
 *
 * Allow modules to define metadata about entity properties.
 */
function data_entity_entity_property_info() {
  $tables = data_entity_get_entity_tables();
  $info = array();
  foreach ($tables as $table) {
    foreach ($table->table_schema['fields'] as $field_name => $field) {
      // Deduce the property type from the schema.
      if ($type = _entity_metadata_convert_schema_type($field['type'])) {
        $info['data_' . $table->name]['properties'][$field_name] = array(
          'label' => !empty($table->meta['fields'][$field_name]['label']) ? $table->meta['fields'][$field_name]['label'] : $field_name,
          'description' => 'Field of type ' . $field['type'] . '.',
          'type' => $type,
          'getter callback' => 'entity_property_verbatim_get',
          'setter callback' => 'entity_property_verbatim_set',
          'schema field' => $field_name,
        );
      }
    }
  }
  return $info;
}
