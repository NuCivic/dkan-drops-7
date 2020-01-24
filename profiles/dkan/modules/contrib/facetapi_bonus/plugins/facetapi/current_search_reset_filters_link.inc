<?php

/**
 * @file
 * Current Search plugin to provide Reset filters link.
 */

class FacetapiBonusCurrentSearchResetFiltersLink extends CurrentSearchItemText {
  /**
   * Implements CurrentSearchItem::execute().
   */
  public function execute(FacetapiAdapter $adapter) {
    $data = array('facetapi_adapter' => $adapter);

    // Determines plurality of string.
    if ($this->settings['plural']) {
      $condition = '[' . $this->settings['plural_condition'] . ']';
      $count = (int) token_replace($condition, $data);
      $raw_text = ($count != 1) ? $this->settings['text_plural'] : $this->settings['text'];
    }
    else {
      $raw_text = $this->settings['text'];
    }

    // Translates text, returns themed output.
    $translated_text = $this->translate('text', $raw_text);

    // Makes sure facet builds are initialized.
    $adapter->processFacets();

    // If no facets were selected stop here.
    $items = $adapter->getAllActiveItems();
    if (empty($items)) {
      return;
    }

    // Get the right URL processor.
    $processor = $adapter->getUrlProcessor();

    // Build the query parameters and remove the current facet key.
    $query = $processor->normalizeParams($processor->fetchParams());
    unset($query[$processor->getFilterKey()]);

    // Get the path from either a basePath or the first search item if the
    // current URL processor doesn't provide a basePath.
    $path = method_exists($processor, 'getBasePath') ? $processor->getBasePath() : $this->getFacetPath(reset($items), $adapter);

    $variables = array(
      'text' => l(token_replace($translated_text, $data), $path, array('query' => $query)),
      'wrapper' => $this->settings['wrapper'],
      'element' => $this->settings['element'],
      'css' => $this->settings['css'],
      'class' => current_search_get_classes($this->settings['classes'], $data),
      'options' => array('html' => TRUE),
    );
    return array('#markup' => theme('current_search_text', $variables));
  }
}
