<?php

/**
 * @file
 * Documentation of FacetAPI Bonus hooks.
 */

/**
 * Hook to rewrite facet items: items of facets can be rewritten prior to rendering.
 *
 * @param array $build
 *  An array of facet items you can rewrite.
 * @param $settings
 *  Contains the facet filter settings as context to determine the facet and the search context.
 */
function HOOK_facet_items_alter(array &$build, &$settings) {
  // We're checking for our "Facet" and if the FacetAPI realm is "block".
  // For further details about FacetAPI realms, see: hook_facetapi_realm_info().
  if ($settings->facet == "YOUR_FACET_NAME" && $settings->realm == 'block') {

    // For each facat item, just rewrite its' markup.
    // For Taxonomy-based Facets, the item $key is (usually) the TermID.
    foreach($build as $key => $item) {
      $build[$key]["#markup"] = drupal_strtoupper($item["#markup"]);
    }
  }
}
