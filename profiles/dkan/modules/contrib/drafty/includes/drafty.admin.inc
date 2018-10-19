<?php

/**
 * @file
 * Drafty Admin settings file.
 */

/**
 * Drafty Form.
 */
function drafty_form($form, &$form_state) {
  $form['revision_cleanup'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['revision_cleanup']['drafty_delete_old_revisions'] = array(
    '#title' => t('Enable deleting redundant revisions'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('drafty_delete_old_revisions', FALSE),
    '#description' => t('Redundant older revisions (those that are marked published->published but are not current) will be deleted when a new revision is saved.'),
  );

  $form['revision_cleanup']['drafty_delete_with_cron'] = array(
    '#title' => t('Delete redundant revisions on cron'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('drafty_delete_with_cron', TRUE),
    '#description' => t('Defer deletion of redundant revisions to when cron runs.'),
    '#states' => array(
      'visible' => array(':input[name="drafty_delete_old_revisions"]' => array('checked' => TRUE)),
    ),
  );

  return system_settings_form($form);
}
