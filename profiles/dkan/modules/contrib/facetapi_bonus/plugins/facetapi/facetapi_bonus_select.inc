<?php

class FacetapiBonusSelect extends FacetapiWidget {
  /**
   * Renders the form.
   */
  public function execute() {
    $element = &$this->build[$this->facet['field alias']];
    $parts = explode(':', $this->build['#settings']->name);
    $name = array_pop($parts);

    $element = drupal_get_form('facetapi_bonus_select_' .  $name, $element);
  }
}


/**
 * Generate form for facet.
 */
function facetapi_bonus_select($form, &$form_state, $elements) {
  $default_value = '';

  // Build options from facet elements.
  $options = array();
  foreach ($elements as $element) {
    $key = serialize(array('path' => $element['#path'], 'query' => $element['#query']));
    $options[$key] = theme('facetapi_bonus_select_option', array('title' => $element['#markup'], 'count' => $element['#count']));

    if ($element['#active']) {
      $default_value = $key;
    }
  }
  if (count($options) == 1) {
    return;
  }
  $form['select'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#attributes' => array('class' => array('ctools-auto-submit')),
    '#default_value' => $default_value,
  );
  if (empty($default_value)) {
    $form['select']['#empty_option'] = t('- Select -');
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#attributes' => array('class' => array('ctools-use-ajax', 'ctools-auto-submit-click')),
    '#submit' => array('facetapi_bonus_select_submit'),
  );

  // Lets add autosubmit js functionality from ctools.
  $form['#attached']['js'][] = drupal_get_path('module', 'ctools') . '/js/auto-submit.js';
  // Add javascript that hides Filter button.
  $form['#attached']['js'][] = drupal_get_path('module', 'facetapi_bonus') . '/js/facetapi_bonus_hide_submit.js';

  $form['#attributes']['class'][] = 'facetapi-bonus-select-facet';

  return $form;
}

/**
 * Submit handler for facet form.
 */
function facetapi_bonus_select_submit($form, &$form_state) {
  $value = unserialize($form_state['values']['select']);
  $form_state['redirect'] = array($value['path'], array('query' => $value['query']));
}
