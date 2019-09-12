<?php

/**
 * @file
 * Provide views access for fieldable panel panes.
 */

/**
 * Fieldable Panels Panes type Views access plugin class.
 */
class fieldable_panels_panes_access_plugin extends views_plugin_access {

  /**
   * {@inheritdoc}
   */
  public function summary_title() {
    return t('Custom access plugin');
  }

  /**
   * {@inheritdoc}
   */
  public function access($account) {
    return fieldable_panels_panes_access_callback();
  }

  /**
   * {@inheritdoc}
   */
  public function get_access_callback() {
    return array('fieldable_panels_panes_access_callback', array());
  }

}
