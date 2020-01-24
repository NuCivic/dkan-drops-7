<?php

/**
 * @file
 * File hooks implemented by the Media: YouTube module.
 */

/**
 * Implements hook_file_operations().
 */
function media_youtube_file_operations() {
  $operations = array(
    'media_youtube_refresh' => array(
      'label' => t('Refresh YouTube information from source'),
      'callback' => 'media_youtube_cache_clear',
    ),
  );

  return $operations;
}

/**
 * Clear the cached YouTube content for the selected files.
 */
function media_youtube_cache_clear($fids) {
  $fids = array_keys($fids);
  $folder = variable_get('youtube_thumb_dir');

  $query = new EntityFieldQuery();
  $results = $query
  ->entityCondition('entity_type', 'file')
  ->propertyCondition('uri', '%' . $folder . '%', 'LIKE')
  ->propertyCondition('fid', $fids)
  ->execute();

  if (!empty($results)) {
    $files = file_load_multiple(array_keys($results['file']));

    foreach ($files as $file) {
      foreach (image_styles() as $isid => $style) {
        $path = image_style_url($isid, $file->uri);
        if ($path) {
          image_path_flush($path);
        }
      }
      drupal_set_message(t('Refreshed thumbnail and derivatives for %filename', array('%filename' => $file->filename)));
    }
  }
}
