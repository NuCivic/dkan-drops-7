<?php 
/**
 * @file
 * Admin settings form for the Panels curator module.
 */

/**
 * Custom settings for the Panels curator module.
 */
function panels_curator_settings_form() {
  module_load_include('inc', 'ctools', 'includes/plugins');
  module_load_include('inc', 'panels', 'includes/plugins');
  module_load_include('inc', 'panels', 'includes/common');
  // Get all panel types.
  $panels_renderer = new panels_renderer_editor;
  $types = panels_common_get_allowed_types('');
  $type_mapping = array();
  foreach ($types as $group => $type) {
    foreach ($type as $key => $panel) {
      $type_mapping[$panel['title']] = array('group' => $group, 'panel' => $key);
    }
  }
  // Load all layouts and regions.
  $regions = array();
  $layouts = panels_get_layouts();
  $display = panels_new_display();
  foreach ($layouts as $layout) {
    foreach (panels_get_regions($layout, $display) as $key => $region) {
      $regions[$key] = $region . ' (' . $layout['title'] . ')';
    }
  }
  $categories = variable_get('panels_curator_categories', '');
  $categories = array_filter(explode(',', $categories));
  if (empty($categories)) {
    // Default to panels default categories.
    $categories = $panels_renderer->get_categories($types);
    // Add 'Disabled' group.
    $categories['disabled'] = array(
      'title' => t('Disabled'),
      'content' => array(),
    );
    $option_keys = array_keys($categories);
  }
  else {
    $option_keys = $categories;
  }
  $options = array();
  foreach ($option_keys as $value) {
    $options[$value] = $value;
  }

  $form['add'] = array(
    '#title' => t('Add a custom group'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );
  $form['add']['custom_group'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
  );

  foreach ($categories as $key => $category) {
    if (!is_array($category)) {
      $key = $category;
      $category = variable_get('panels_curator_' . $key, '');
      $category = unserialize($category);
    }
    $category_title = check_plain($category['title']) . ' (' . count($category['content']) . ')';
    $form[$key] = array(
      '#title' => $category_title,
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    // Region settings for this category/group.
    $category['regions'] = variable_get('panels_curator_regions_' . $key, '');
    $category['regions'] = explode(',', $category['regions']);
    $form[$key][$key . '_region_container'] = array(
      '#title' => t('Regions'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$key][$key . '_region_container'][$key . '_regions'] = array(
      '#title' => t('Regions'),
      '#type' => 'checkboxes',
      '#options' => $regions,
      '#default_value' => isset($category['regions']) ? $category['regions'] : array(),
    );
    foreach ($category['content'] as $content) {
      $form[$key][$content['title']] = array(
        '#title' => $content['title'],
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $key,
      );
      // Store the serialized data in a hidden variable.
      $form['hidden_' . $content['title']] = array(
        '#title' => $content['title'],
        '#type' => 'hidden',
        '#default_value' => $key,
        '#value' => serialize($content),
      );
      // Keep track of any new or un-categorized panels.
      if (isset($type_mapping[$content['title']])) {
        unset($type_mapping[$content['title']]);
      }
    }
  }

  // Add any new or un-categorized panels.
  foreach ($type_mapping as $key => $type) {
    $group = 'disabled';
    if (isset($form[$type['group']])) {
      $group = $type['group'];
    }
    $panel_title = $types[$type['group']][$type['panel']]['title'];
    $form[$group][$panel_title] = array(
      '#title' => $panel_title,
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $group,
    );
    // Store the serialized data in a hidden variable.
    $form['hidden_' . $panel_title] = array(
      '#title' => $panel_title,
      '#type' => 'hidden',
      '#default_value' => $group,
      '#value' => serialize($types[$type['group']][$type['panel']]),
    );
  }

  $form['#submit'][] = 'panels_curator_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Custom settings for the Panels curator module.
 */
function panels_curator_settings_form_submit($form, &$form_state) {
  // Set curated categories.
  $category_keys = variable_get('panels_curator_categories', '');
  $category_keys = array_filter(explode(',', $category_keys));
  if (empty($category_keys)) {
    module_load_include('inc', 'panels', 'includes/common');
    // Default to panels default categories.
    $panels_renderer = new panels_renderer_editor;
    $types = panels_common_get_allowed_types('');
    $categories = $panels_renderer->get_categories($types);
    // Add 'Disabled' group.
    $categories['disabled'] = array(
      'title' => t('Disabled'),
      'content' => array(),
    );
  }
  else {
    // Load category mappings from existing variables.
    $categories = array();
    foreach ($category_keys as $category) {
      if (!is_array($category)) {
        $key = $category;
        $category = variable_get('panels_curator_' . $key, '');
        $categories[$key] = unserialize($category);
      }
    }
  }
  // Create a new group.
  if (!empty($form_state['values']['custom_group'])) {
    $group_title = preg_replace('@[^a-z0-9-]+@','-', strtolower($form_state['values']['custom_group']));
    $categories[$group_title] = array(
      'title' => $form_state['values']['custom_group'],
      'content' => array(),
    );
    variable_set('panels_curator_' . $group_title, serialize($categories[$group_title]));
  }
  $category_groups = array();
  foreach (array_keys($categories) as $key => $value) {
    $category_groups[] = $value;
  }
  // Set the new update categories.
  variable_set('panels_curator_categories', implode(',', $category_groups));
  // Check for updates to each group.
  $category_mappings = array();
  foreach ($form_state['input'] as $key => $value) {
    // Look for all panel panes that can be added to a group.
    if (!empty($value) && isset($form_state['input']['hidden_' . $key])) {
      // Initialize the category mapping for this group.
      if (!isset($category_mappings[$value])) {
        $category_mappings[$value] = array(
          'title' => $categories[$value]['title'],
          'content' => array(),
        );
      }
      $category_mappings[$value]['content'][$key] = unserialize($form_state['input']['hidden_' . $key]);
    }
  }
  // Set each category mapping with associated panel panes.
  foreach ($category_mappings as $key => $mapping) {
    variable_set('panels_curator_' . $key, serialize($mapping));
  }
  foreach ($category_groups as $group) {
    // Update all category panels mappings.
    if (!isset($category_mappings[$group])) {
      $empty = array(
        'title' => $categories[$group]['title'],
        'content' => array(),
      );
      variable_set('panels_curator_' . $group, serialize($empty));
    }
    // Update all category region mappings.
    $region_mappings = array();
    foreach ($form_state['input'][$group . '_regions'] as $key => $region) {
      if (!empty($region)) {
        $region_mappings[$key] = $region;
      }
    }
    variable_set('panels_curator_regions_' . $group, implode(',', $region_mappings));
  }
}
