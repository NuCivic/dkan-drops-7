<?php

/**
 * @file
 *
 * Filter to only show items from a specific depth of a hierarchical set of
 * facet items.
 */

/**
 * Plugin that excludes specified facet items.
 */
class FacetapiFilterShowItemsFromSpecificDepth extends FacetapiFilter {

  // Filtered build.
  protected $build;

  /**
   * Filters items.
   */
  public function execute(array $build) {

    if ($this->settings->settings['parents']) {
      $this->build = $build;
      $this->filterDepthParents($this->build);
    }
    else {
      $this->build = array();
      $this->filterDepth($build);
    }

    return $this->build;
  }

  /**
   * Collect only items that do not have children.
   */
  protected function filterDepth($items, $current_depth = 0) {
    foreach ($items as $key => $item) {
      if ($current_depth == $this->settings->settings['depth']) {
        // If this item has children we just unset it
        if (!empty($item['#item_children'])) {
          unset($item['#item_children']);
        }
        $this->build[$key] = $item;
      }
      elseif (!empty($item['#item_children'])) {
        $this->filterDepth($item['#item_children'], $current_depth + 1);
      }
    }
  }

  /**
   * Collect only items that do not have children.
   */
  protected function filterDepthParents(&$items, $current_depth = 0) {
    foreach ($items as $key => &$item) {
      if ($current_depth == $this->settings->settings['depth']) {
        // If this item has children we just unset it
        if (!empty($item['#item_children'])) {
          unset($item['#item_children']);
        }
      }
      elseif (!empty($item['#item_children'])) {
        $this->filterDepthParents($item['#item_children'], $current_depth + 1);
      }
    }
  }

  /**
   * Adds settings to the filter form.
   */
  public function settingsForm(&$form, &$form_state) {
    $form['depth'] = array(
      '#title' => t('Depth'),
      '#type' => 'textfield',
      '#description' => t("Only show items from this depth of a hierarchical set of facet items."),
      '#default_value' => $this->settings->settings['depth'],
    );
    $form['parents'] = array(
      '#title' => t('Keep parents'),
      '#type' => 'checkbox',
      '#description' => t('Keep the parents of the depth intact. With this checked, the logic is more along the lines of "Filter Items Up To A Specific Depth"'),
      '#default_value' => $this->settings->settings['parents'],
    );
  }

  /**
   * Returns an array of default settings.
   */
  public function getDefaultSettings() {
    return array(
      'depth' => 0,
      'parents' => 0,
    );
  }
}
