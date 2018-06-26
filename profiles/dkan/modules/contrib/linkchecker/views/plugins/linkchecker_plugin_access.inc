<?php

/**
 * @file
 * Definition of linkchecker_plugin_access.
 */

/**
 * Access plugin for the linkchecker module.
 *
 * @ingroup views_access_plugins
 */
class linkchecker_plugin_access extends views_plugin_access {

  /**
   * {@inheritdoc}
   */
  function access($account) {
    return _linkchecker_access($account);
  }

  /**
   * {@inheritdoc}
   */
  function get_access_callback() {
    return array('_linkchecker_access', array());
  }

  /**
   * {@inheritdoc}
   */
  function summary_title() {
    return t('Linkchecker');
  }

}
