<?php

/**
 * @file
 * Provide views handlers for fieldable panel panes.
 */

/**
 * Field handler to present a link to the node.
 */
class fieldable_panels_panes_handler_field_view_entity extends views_handler_field_entity {

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    $options['hide_unless_reusable'] = array('default' => FALSE, 'translatable' => FALSE);
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    $form['hide_unless_reusable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide link for non-reusable panes'),
      '#description' => t("If you're using revision locking, it can be useful to hide links on non-reusable panes."),
      '#default_value' => $this->options['hide_unless_reusable'],
    );
    parent::options_form($form, $form_state);

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    if ($entity = $this->get_value($values)) {
      return $this->render_link($entity, $values);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function render_link($entity, $values) {
    if (fieldable_panels_panes_access('view', $entity) && (empty($this->options['hide_unless_reusable']) || !empty($entity->reusable))) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "admin/structure/fieldable-panels-panes/view/$entity->fpid";
      $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
      return $text;
    }
    else {
      $this->options['alter']['path'] = '';
    }
  }

}
