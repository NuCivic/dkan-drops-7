<?php

/**
 * @file
 * A facet handler that falls back onto session set variables.
 */

/**
 * Url processor plugin that retrieves facet data from the query string.
 *
 * This plugin retrieves facet data from $_SESSION. $_SESSION is populated
 * from $_GET, storing all information in the "f" query string variable by
 * default.
 */
class FacetapiUrlProcessorSession extends FacetapiUrlProcessorStandard {

  /**
   * Stores the calculated namespace for this request.
   */
  static protected $namespace;

  /**
   * Implements FacetapiUrlProcessor::fetchParams().
   *
   * Use $_GET as the source for facet data when available and fall back to
   * $_SESSION, also storing the $_GET bits into the session.
   */
  public function fetchParams() {
    $return = parent::fetchParams();
    // Get hold of our namespaced session variable.
    $session =& $this->getSession();

    // Check if we need to update the session with a new request.
    if (isset($_GET[$this->filterKey])) {
      $session[$this->filterKey] = $_GET[$this->filterKey] == 'clear' ? array() : $_GET[$this->filterKey];

      // Strip out any empty filters.
      foreach ($session[$this->filterKey] as $i => $facet) {
        $parts = explode(':', $facet);
        if (empty($parts[1])) {
          unset($session[$this->filterKey][$i]);
        }
      }
    }

    return array_merge($return, $session);
  }

  /**
   * Implements FacetapiUrlProcessor::setBreadcrumb().
   */
  public function setBreadcrumb() {
    // Do nothing.
  }

  /**
   * Implements FacetapiUrlProcessor::getQueryString().
   */
  public function getQueryString(array $facet, array $values, $active) {
    // Let our parent do the processing.
    $query = parent::getQueryString($facet, $values, $active);

    // We need a special way to clear facets stored in session.
    if (empty($query[$this->filterKey])) {
      $query[$this->filterKey] = 'clear';
    }

    return $query;
  }

  /**
   * Get the namespaced session variable.
   */
  protected function &getSession() {
    $namespace = self::getNamespace();

    // If we have a namespace, return the session variable by reference.
    if ($namespace) {
      // We also namespace by adapter to prevent un-wanted crossover.
      if (!isset($_SESSION['facetapi']['url_processor_session'][$this->adapter->getSearcher()][$namespace])) {
        $_SESSION['facetapi']['url_processor_session'][$this->adapter->getSearcher()][$namespace] = array();
      }
      return $_SESSION['facetapi']['url_processor_session'][$this->adapter->getSearcher()][$namespace];
    }

    // Otherwise create a variable we can return by reference.
    $session = array();
    return $session;
  }

  /**
   * Sniff out a namespace.
   */
  static public function getNamespace() {
    if (!isset(self::$namespace)) {
      // Set it to false in case we don't find one.
      self::$namespace = FALSE;

      // Define our patterns.
      // @TODO move this into an admin UI.
      $patterns = array(
        'site' => '*',
      );

      // Iterate over our patterns in order and take the first match.
      // @TODO figure out something better than first match.
      foreach ($patterns as $name => $pattern) {
        if (drupal_match_path(current_path(), $pattern)) {
          self::$namespace = $name;
          break;
        }
      }
    }

    return self::$namespace;
  }

  /**
   * Clear the session.
   *
   * @param $searcher
   *   The name of the searcher that we want to clear. If NULL we'll clear
   *   them all.
   * @param $namespace
   *   Leave NULL to get the current namespace, TRUE to clear all or specify
   *   a namespace as a string to target a specific one.
   */
  static public function clearSession($searcher = NULL, $namespace = NULL) {
    if ($namespace == NULL) {
      $namespace = self::getNamespace();
    }

    if ($searcher) {
      if ($namespace === TRUE) {
        // Entirely clear this searcher.
        unset($_SESSION['facetapi']['url_processor_session'][$searcher]);
      }
      else {
        // Clear a specific namespace.
        unset($_SESSION['facetapi']['url_processor_session'][$searcher][$namespace]);
      }
    }
    elseif ($namespace === TRUE) {
      // Clear everything.
      unset($_SESSION['facetapi']['url_processor_session']);
    }
    elseif ($namespace) {
      // Clear a specific namespace in all searchers.
      if (isset($_SESSION['facetapi']['url_processor_session'])) {
        foreach ($_SESSION['facetapi']['url_processor_session'] as &$searcher) {
          unset($searcher[$namespace]);
        }
      }
    }
  }

}
