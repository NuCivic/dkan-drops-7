<?php

/**
 * @file
 * Definition of linkchecker_handler_filter_linkchecker_link_code.
 */

/**
 * Filter by http status code.
 *
 * @ingroup views_filter_handlers
 */
class linkchecker_handler_filter_linkchecker_link_code extends views_handler_filter_in_operator {

  /**
   * {@inheritdoc}
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = _linkchecker_response_codes();
  }

}
