<?php

/**
 * @file
 * Definition of linkchecker_handler_field_linkchecker_link_link_edit.
 */

/**
 * Field handler to present a linkchecker edit link.
 *
 * @ingroup views_field_handlers
 */
class linkchecker_handler_field_linkchecker_link_link_edit extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  function construct() {
    parent::construct();

    $this->additional_fields['lid'] = 'lid';
  }

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['destination'] = array('default' => TRUE, 'bool' => TRUE);
    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );

    parent::options_form($form, $form_state);

    $form['destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use destination'),
      '#description' => t('Add destination to the link'),
      '#default_value' => $this->options['destination'],
      '#fieldset' => 'more',
    );

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  /**
   * {@inheritdoc}
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * {@inheritdoc}
   */
  function render($values) {
    // Check there is an actual value, as on a relationship there may not be.
    if ($lid = $this->get_value($values, 'lid')) {
      return $this->render_link($lid);
    }
  }

  /**
   * Renders the link.
   */
  function render_link($lid) {
    if (_linkchecker_access_edit_link_settings(array('lid' => $lid))) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'linkchecker/' . $lid . '/edit';

      if (!empty($this->options['destination'])) {
        $this->options['alter']['query'] = drupal_get_destination();
      }

      $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit link settings');

      return $text;
    }
  }

}
