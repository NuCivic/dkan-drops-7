<?php

/**
 * @file
 * Drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function autoload_drush_command() {
  $commands = array();

  $commands['autoload-rebuild'] = array(
    'description' => dt('Rebuild the autoloading class map.'),
    'aliases' => array('aur'),
  );

  return $commands;
}

/**
 * Implements hook_drush_exit().
 */
function autoload_drush_exit() {
  $command = drush_get_command();

  // Rebuild the autoloading class map only after downloading a bunch of
  // projects instead of doing this for every single one inside of the.
  /* @see hook_drush_pm_post_download() */
  if ('pm-download' === $command['command']) {
    drush_autoload_rebuild();
  }
}

/**
 * Implements drush_COMMAND().
 */
function drush_autoload_rebuild() {
  if (!db_table_exists(AutoloadCache::BIN)) {
    drupal_install_schema('autoload');
  }

  autoload(TRUE);
  drush_log(dt('The autoloading class map has been rebuilt.'), 'success');
}
