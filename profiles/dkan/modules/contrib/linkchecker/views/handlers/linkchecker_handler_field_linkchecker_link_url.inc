<?php

/**
 * @file
 * Definition of linkchecker_handler_field_linkchecker_link_url.
 */

/**
 * Field handler to present a linkchecker url.
 *
 * @ingroup views_field_handlers
 */
class linkchecker_handler_field_linkchecker_link_url extends views_handler_field {

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
  function render($values) {
    // Check there is an actual value, as on a relationship there may not be.
    if ($lid = $this->get_value($values, 'lid') && $url = $this->get_value($values)) {
      $link = (object) array('lid' => $lid, 'url' => $url);
      return $this->render_link($link);
    }
  }

  /**
   * Renders the link.
   */
  function render_link($link) {
    if (user_access('access broken links report') || _linkchecker_link_access($link)) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $link->url;
      return $link->url;
    }
    else {
      return t('Permission restrictions deny you access to this broken link.');
    }
  }

}
