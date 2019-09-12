<?php

/**
 * @file
 * Hooks provided by the Fieldable Panels Panes module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Respond to fieldable panels pane deletion.
 *
 * @param object $panels_pane
 *   The fieldable panels pane that is being deleted.
 *
 * @ingroup fieldable_panels_pane_api_hooks
 */
function hook_fieldable_panels_pane_delete($panels_pane) {
  db_delete('mytable')
    ->condition('fpid', $panels_pane->fpid)
    ->execute();
}

/**
 * Respond to creation of a new fieldable panels pane.
 *
 * @param object $panels_pane
 *   The fieldable that is being created.
 *
 * @ingroup fieldable_panels_pane_api_hooks
 */
function hook_fieldable_panels_pane_insert($panels_pane) {
  db_insert('mytable')
    ->fields(array(
      'fpid' => $panels_pane->fpid,
      'vid' => $panels_pane->vid,
    ))
    ->execute();
}

/**
 * Act on a fieldable panels pane being inserted or updated.
 *
 * @param object $panels_pane
 *   The fieldable panels pane that is being inserted or updated.
 *
 * @ingroup fieldable_panels_pane_api_hooks
 */
function hook_fieldable_panels_pane_presave($panels_pane) {
  // @todo: Needs example.
}

/**
 * Respond to updates to a fieldable panels pane.
 *
 * @param object $panels_pane
 *   The fieldable panels pane that is being updated.
 *
 * @ingroup fieldable_panels_pane_api_hooks
 */
function hook_fieldable_panels_pane_update($panels_pane) {
  db_update('mytable')
    ->fields(array('fpid' => $panels_pane->fpid))
    ->condition('vid', $panels_pane->vid)
    ->execute();
}

/**
 * Act on a fieldable panels pane that is being assembled before rendering.
 *
 * @param object $panels_pane
 *   The fieldable panels pane that is being assembled for rendering.
 * @param string $view_mode
 *   The $view_mode parameter.
 * @param string $langcode
 *   The language code used for rendering.
 *
 * @see hook_entity_view()
 *
 * @ingroup fieldable_panels_pane_api_hooks
 */
function hook_fieldable_panels_pane_view($panels_pane, $view_mode, $langcode) {
  $panels_pane->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * All the list of CTools plugin specs for FPP objects to be modified.
 *
 * @param array $types
 *   All of the CTools plugin specifications for these FPP objects.
 * @param string $bundle
 *   The FPP bundle.
 * @param array $entities
 *   All of the FPP entities for this bundle indexed by their CTools subtype,
 *   e.g. fpid:123, vid:123, uuid:123.
 */
function hook_fieldable_panels_panes_content_types_alter(array &$types, $bundle, array $entities) {
  foreach ($types as $name => &$type) {
    $type['icon'] = 'icon_funnyface.png';
  }
}

/**
 * Allow other modules to control access to Fieldable Panels Pane objects.
 *
 * @param string $op
 *   The operation to be performed.
 * @param object $entity
 *   The fieldable panels pane that is being accessed.
 * @param object|null $account
 *   The user account whose access should be checked.
 *
 * @return bool|null
 *   Returns TRUE to allow access, FALSE to deny, or NULL to pass the access
 *   decision off to the next hook or the module itself.
 *
 * @ingroup fieldable_panels_pane_api_hooks
 */
function hook_fieldable_panels_panes_access($op, $entity = NULL, $account = NULL) {
  // Example implementation which restricts access to edit reusable panes.
  if ($op == 'update' && !empty($entity) && $entity->reusable && !user_access('administer fieldable panels panes')) {
    return FALSE;
  }
  return NULL;
}

/**
 * Allow other modules to modify access to the FPP CTools content type.
 *
 * @param bool $return
 *   Value to determine if edit access is granted to FPP entity.
 * @param array $content_type
 *   The CTools content type plugin.
 * @param array $subtype
 *   The individual FPP entity being evaluated for edit access.
 * @param array $view_mode
 *   The view mode of the FPP entity being evaluated for edit access.
 */
function hook_fieldable_panels_pane_content_type_edit_form_access_alter(&$return, array $content_type, array $subtype, array $view_mode) {
  // For button and quote FPP bundles, deny edit access from Panels.
  if ($subtype['bundle'] == 'button' || $subtype['bundle'] == 'quote') {
    $return = FALSE;
  }
}

/**
 * Allow other modules to modify the Fieldable Panels Pane CTools content type.
 *
 * @param array $content_type
 *   The individual content type to be returned.
 * @param string $subtype_id
 *   The subtype id of the fieldable panel pane being altered for render.
 * @param array $plugin
 *   The CTools content type plugin.
 */
function hook_fieldable_panels_pane_content_type_alter(array &$content_type, $subtype_id, array $plugin) {
  // For button FPP bundles, always show the latest revision.
  if ($content_type['bundle'] == 'button' && substr($subtype_id, 0, 4) === 'vid:') {
    $vid = substr($subtype_id, strpos($subtype_id, ':') + 1);
    $fpid = db_query('SELECT f.fpid FROM {fieldable_panels_panes} f WHERE f.vid = :vid', array(':vid' => $vid))->fetchField();
    $content_type['name'] = 'fpid:' . $fpid;
    $content_type['entity_id'] = 'fpid:' . $fpid;
  }
}

/**
 * @} End of "addtogroup hooks".
 */
