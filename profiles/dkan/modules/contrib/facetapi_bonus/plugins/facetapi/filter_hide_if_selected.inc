<?php

/**
 * @file
 *
 * FacetAPI Filter that removes the current facet if it has at least one
 * item selected.
 */

/**
 * Plugin that hides the facet if it has an item already selected.
 */
class FacetapiFilterHideIfSelected extends FacetapiFilter {

  /**
   * Filters the items.
   */
  public function execute(array $build) {
    if (self::hasActiveItems($build)) {
      return array();
    }

    return $build;
  }

  /**
   * Check if any of the given facet items are selected.
   *
   * @param array $items
   *   The facet items.
   * @return bool
   */
  protected static function hasActiveItems($items) {
    foreach ($items as $item) {
      if ($item['#active']) {
        return TRUE;
      }
    }

    return FALSE;
  }
}
