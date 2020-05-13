<?php

/**
 * @file
 * Provides a panels_storage plugin for Panelizer defaults.
 */

// Plugin definition
$plugin = array(
  'access callback' => 'panelizer_default_panels_storage_access',
);

/**
 * Access callback for panels storage.
 */
function panelizer_default_panels_storage_access($storage_type, $storage_id, $op, $account) {
  list ($entity_type, $entity_id) = explode(':', $storage_id);
  $entity = current(entity_load($entity_type, array($entity_id)));
  list(, , $bundle) = entity_extract_ids('node', $entity);

  if ($op == 'change layout') {
    return user_access("administer panelizer $entity_type $bundle defaults", $account) || user_access("administer panelizer $entity_type $bundle layout", $account);
  }
  else {
    return user_access("administer panelizer $entity_type $bundle defaults", $account) || user_access("administer panelizer $entity_type $bundle content", $account);
  }
}
