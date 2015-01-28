<?php
/**
 * @file
 * Theme and preprocess functions for breadcrumbs, messages, tabs..etc.
 */

/**
 * Implements hook_css_alter().
 */
function nuboot_radix_css_alter(&$css) {
  $radix_path = drupal_get_path('theme', 'radix');

  // Radix now includes compiled stylesheets for demo purposes.
  // We remove these from our subtheme since they are already included
  // in compass_radix.
  unset($css[$radix_path . '/assets/stylesheets/radix-style.css']);
  unset($css[$radix_path . '/assets/stylesheets/radix-print.css']);
  // Remove dkan button styles so we can use our own.
  unset($css[drupal_get_path('module', 'dkan_dataset') . '/dkan_dataset_btn.css']);
  unset($css[drupal_get_path('module', 'dkan_dataset') . '/dkan_dataset.css']);
}

/**
 * Implements template_preprocess_page().
 */
function nuboot_radix_preprocess_page(&$variables) {
  // Add copyright to theme.
  if ($copyright = theme_get_setting('copyright')) {
    $variables['copyright'] = isset($copyright['value']) ? check_markup($copyright['value'], $copyright['format']) : '';
  }
  if (drupal_is_front_page()) {
    $vars['title'] = '';
    unset($vars['page']['content']['system_main']['default_message']);
  }
  // Remove title on dataset edit and creation pages.
  if (!empty($vars['node']) && in_array($vars['node']->type, array('dataset', 'resource')) || arg(1) == 'add') {
    $vars['title'] = '';
  }
  // Add external css files.
  $external_css = array(
    '#attached' => array(
      'css' => array(
        '//fonts.googleapis.com/css?family=Droid+Sans:400,700|Open+Sans:300italic,400italic,600italic,700italic,800italic,400,300,600,700,800' => array(
          'type' => 'external'),
      ),
    ),
  );
  drupal_render($external_css);
}
