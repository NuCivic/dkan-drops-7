<?php

/**
 * @file
 * Class for the Panelizer fieldable_panels_pane term entity plugin.
 */

/**
 * Panelizer Entity fieldable_panels_pane term plugin class.
 *
 * Handles term specific functionality for Panelizer.
 */
class FieldablePanelsPaneEntity extends PanelizerEntityDefault {

  /**
   * {@inheritdoc}
   */
  public $entity_admin_root = 'admin/structure/fieldable-panels-panes/%fieldable_panels_pane_type';

  /**
   * {@inheritdoc}
   */
  public $entity_admin_bundle = 3;

  /**
   * {@inheritdoc}
   */
  public $supports_revisions = TRUE;

  /**
   * {@inheritdoc}
   */
  public $views_table = 'fieldable_panels_panes';

  /**
   * {@inheritdoc}
   */
  public function entity_access($op, $entity) {
    return fieldable_panels_panes_access($op, $entity);
  }

  /**
   * {@inheritdoc}
   */
  public function entity_save($entity) {
    return fieldable_panels_panes_save($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function entity_identifier($entity) {
    return t('This pane');
  }

  /**
   * {@inheritdoc}
   */
  public function entity_bundle_label() {
    return t('Pane bundle');
  }

  /**
   * {@inheritdoc}
   */
  public function get_default_display($bundle, $view_mode) {
    return parent::get_default_display($bundle, $view_mode);
  }

}
