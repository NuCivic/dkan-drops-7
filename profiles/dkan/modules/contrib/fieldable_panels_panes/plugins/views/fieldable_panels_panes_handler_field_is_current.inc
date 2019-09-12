<?php

/**
 * @file
 * Provide views handlers for fieldable panel panes.
 */

/**
 * Field handler to display if the revision is current or not.
 */
class fieldable_panels_panes_handler_field_is_current extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  public $entities;

  /**
   * {@inheritdoc}
   */
  public function construct() {
    parent::construct();
    $this->additional_fields = array('fpid', 'vid');
  }

  /**
   * {@inheritdoc}
   */
  public function pre_render(&$values) {
    $this->entities = array();
    if (!empty($values)) {
      foreach ($values as $row_index => $value) {
        if (!empty($value->{$this->aliases['fpid']}) && !empty($value->{$this->aliases['vid']})) {
          $this->entities[$row_index] = fieldable_panels_panes_load($value->{$this->aliases['fpid']}, $value->{$this->aliases['vid']});

          if (!empty($this->entities[$row_index]) && $this->entities[$row_index]->vid == $this->entities[$row_index]->current_vid) {
            $values[$row_index]->{$this->field_alias} = $this->options['current_text'];
          }
          else {
            $values[$row_index]->{$this->field_alias} = $this->options['not_current_text'];
          }
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['current_text'] = array('default' => 'Current', 'translatable' => TRUE);
    $options['not_current_text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    $form['current_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display if current'),
      '#default_value' => $this->options['current_text'],
    );

    $form['not_current_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display if not current'),
      '#default_value' => $this->options['not_current_text'],
    );

    parent::options_form($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->ensure_my_table();

    // This isn't a real field, but we put it there nonetheless.
    $this->field_alias = $this->table_alias . '_is_current';

    $this->add_additional_fields();
  }

  /**
   * {@inheritdoc}
   */
  public function sanitize_value($value, $type = NULL) {
    return filter_xss_admin($value);
  }

}
