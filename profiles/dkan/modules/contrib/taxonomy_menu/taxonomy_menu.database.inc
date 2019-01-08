<?php


/**
 * @file
 * Database functions
 */

/**
 * Inserts a menu item.
 *
 * @param int $mlid
 *   Menu link ID.
 * @param int $tid
 *   Taxonomy term ID.
 * @param int $vid
 *   Vocabulary ID.
 */
function _taxonomy_menu_insert_menu_item($mlid, $tid, $vid) {
  $fields = array(
    'mlid' => $mlid,
    'tid' => $tid,
    'vid' => $vid,
  );
  db_insert('taxonomy_menu')->fields($fields)->execute();
}

/**
 * Returns the corresponding menu link id.
 *
 * @param int $tid
 *   Taxonomy term ID.
 * @param int $vid
 *   Vocabulary ID.
 * @return int
 *   Menu link ID for that taxonomy term.
 */
function _taxonomy_menu_get_mlid($tid, $vid) {
  $where = array(
    ':tid' => $tid,
    ':vid' => $vid,
  );
  return db_query('SELECT mlid FROM {taxonomy_menu} WHERE tid = :tid AND vid = :vid', $where)->fetchField();
}

/**
 * Retrieves the term / menu relations for a vocab.
 *
 * @param $vid
 *   Vocabulary ID.
 *
 * @return array
 *   Relations to menu link ID as an array keyed by taxonomy term ID.
 *     array(tid => mlid)
 */
function _taxonomy_menu_get_menu_items($vid) {
  return db_query('SELECT tid, mlid FROM {taxonomy_menu} WHERE vid = :vid', array(':vid' => $vid))->fetchAllKeyed();
}

/**
 * Deletes all links associated with this vocab from both the taxonomy_menu
 * table and the menu_link table.
 *
 * @param int $vid
 *   Vocabulary ID.
 */
function _taxonomy_menu_delete_all($vid) {
  $menu_terms = _taxonomy_menu_get_menu_items($vid);
  if (!empty($menu_terms)) {
    foreach ($menu_terms as $tid => $mlid) {
      db_delete('menu_links')
        ->condition('mlid', $mlid)
        ->execute();
    }
    db_delete('taxonomy_menu')
      ->condition('vid', $vid)
      ->execute();
  }
}

/**
 * Gets an array of the tid's related to the node
 *
 * @param object $node
 *   Node object.
 *
 * @return array
 *   Array of taxonomy term IDs.
 */
function _taxonomy_menu_get_node_terms($node) {
  // Get the taxonomy fields.
  $tids = array();
  $result = db_query("SELECT field_name FROM {field_config} WHERE type = 'taxonomy_term_reference'");
  foreach ($result as $field) {
    $field_name = $field->field_name;

    if (isset($node->$field_name)) {
      $tid_field = $node->$field_name;
      // Loop through all the languages.
      foreach ($tid_field as $tid_field_languages) {
        // Loop through all the tids
        foreach ($tid_field_languages as $tid) {
          $tids[] = $tid['tid'];
        }
      }
    }
  }
  return $tids;
}

/**
 * Gets the parent tids for a taxonomy term.
 *
 * @param int $tid
 *   Taxonomy term ID.
 *
 * @return array
 *   Array of taxonomy term IDs.
 */
function _taxonomy_menu_get_parents($tid) {
  $output = array();
  $result = taxonomy_get_parents($tid);
  foreach ($result as $key => $item) {
    $output[] = $key;
  }
  return $output;
}

/**
 * Deletes all rows from {taxomony_menu} associated with this tid
 *
 * @param $vid
 * @param $tid
 * @param int $vid
 *   Vocabulary ID.
 * @param int $tid
 *   Taxonomy term ID.
 */
function _taxonomy_menu_delete_item($vid, $tid) {
  $and = db_and()->condition('vid', $vid)->condition('tid', $tid);
  db_delete('taxonomy_menu')->condition($and)->execute();
}

/**
 * Gets all taxonomy terms for a given vocabulary.
 *
 * @param int $vid
 *   Vocabulary ID.
 *
 * @return array
 *   Array of taxonomy term IDs.
 */
function _taxonomy_menu_get_terms($vid) {
  $result = db_select('taxonomy_term_data', 'td')
    ->condition('vid', $vid)
    ->fields('td', array('tid'))
    ->execute();
  return $result->fetchAll();
}

/**
 * Gets the count of nodes for each term (without children).
 *
 * @param int $tid
 *   Taxonomy term ID.
 *
 * @return int
 *   Count of nodes that reference the term.
 *
 * @todo Needs updating since terms are related via fields now.
 */
function _taxonomy_menu_term_count($tid) {
  $result = db_select('taxonomy_index', 'tn');
  $result->condition('tid', $tid);
  $result->join('node', 'n', 'n.nid = tn.nid AND n.status = 1');
  $result->addExpression('COUNT(n.nid)', 'term_count');
  $temp = $result->execute();
  $temp = $temp->fetchObject();
  return $temp->term_count;
}

/**
 * Gets tid for a given mlid.
 *
 * @param int $mlid
 *   Menu link ID.
 *
 * @return int $tid
 *   Taxonomy term ID.
 */
function _taxonomy_menu_get_tid($mlid) {
  return db_query('SELECT tid FROM {taxonomy_menu} WHERE mlid = :mlid', array(':mlid' => $mlid))->fetchField();
}

/**
 * Gets the vocabulary ID and taxonomy term ID for a given menu link ID.
 *
 * @param int $mlid
 *   Menu link ID.
 *
 * @return array
 *   array(vid, tid)
 */
function _taxonomy_menu_get_item($mlid) {
  $result = db_select('taxonomy_menu', 'tm')
    ->condition('mlid', $mlid, '=')
    ->fields('tm', array('tid', 'vid'))
    ->execute();

  return $result->fetch();
}

/**
 * Gets the vocabulary for a taxonomy term ID.
 *
 * @param array $tids
 *   Taxonomy term IDs.
 *
 * @return $vid
 *   Vocabulary ID.
 */
function _taxonomy_menu_get_vid_by_tid($tids) {
  if ($tids) {
    $result = db_select('term_data')
      ->condition('tid', $tids, 'IN')
      ->fields('term_data', array('vid'))
      ->distinct()
      ->execute();
    return $result->fetchAllAssoc('vid');
  }
}
