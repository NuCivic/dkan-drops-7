<?php

/**
 * @file
 *
 * Filter to rewrite facet items via a dedicated hook
 *
 * TODOs
 * - determine whether the definition and invocation of hook_facet_items_alter
 *   shouldn't happen within FacetAPI directly.
 *   Advantage of this module: The cost of invocation occurs only it needed.
 */

/**
 * Facetapi plugin for rewriting facet items by
 * implementing a hook_facet_items_alter(&$build, &$settings);
 */
class FacetapiFilterRewriteItems extends FacetapiFilter {

  /**
   * Filters facet items.
   */
  public function execute(array $build) {
    drupal_alter('facet_items', $build, $this->settings);
    return $build;
  }

  /**
   * Adds hook documentation
   */
  function settingsForm(&$form, &$form_state) {
    $form['rewrite_items'] = array(
      // This documentation should be a '#type' => 'markup' element, but
      // Facet API throws a unrecoverable PHP error if an plugin settings form
      // element hasn't got a real value.
      // Thus this crude "always checked" checkbox.
      '#title' => t('Rewrite facet items (always checked)'),
      '#type' => 'markup',
      '#default_value' => TRUE,
      '#markup' => t('By enabling this filter, items of this facet can be rewritten prior to rendering by implementing the hook:
<code>function HOOK_facet_items_alter(&$build, &$settings)</code>.
</br>See <code>facetapi_bonus.api.php</code> for further documentation.'),
    );
  }

}
