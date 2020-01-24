<?php

/**
 * @file
 * Contains views_plugin_access_menu.
 */

/**
 * (Deprecated) Access plugin that provides router item-based access control.
 *
 * @ingroup views_access_plugins
 */
class views_plugin_access_menu extends views_plugin_access {

  /**
   * {@inheritdoc}
   */
  public function access($account) {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function get_access_callback() {
    return FALSE;
  }

}
