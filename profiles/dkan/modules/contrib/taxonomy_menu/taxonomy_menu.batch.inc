<?php

/**
 * @file
 * Batch API Code
 */

/**
 *Starts a batch operation.
 *
 * @param int $vid
 *   Vocabulary ID.
 */
function _taxonomy_menu_insert_link_items_batch($vid) {
  $depth = variable_get(_taxonomy_menu_build_variable('max_depth', $vid), 0);
  if ($depth == 0) {
    $depth = NULL;
  }
  $terms = taxonomy_get_tree($vid, 0, $depth, TRUE);
  $menu_name = variable_get(_taxonomy_menu_build_variable('vocab_menu', $vid), FALSE);

  $batch = array(
    // An array of callbacks and arguments for the callbacks.
    'operations' => array(
      array('_taxonomy_menu_insert_link_items_process', array($terms, $menu_name)),
    ),
    // A callback to be used when the batch finishes.
    'finished' => '_taxonomy_menu_insert_link_items_success',
    // A title to be displayed to the end user when the batch starts.
    'title' => t('Rebuilding Taxonomy Menu'),
    // An initial message to be displayed to the end user when the batch starts.
    'init_message' => t('The menu items have been deleted, and are about to be regenerated.'),
    // A progress message for the end user. Placeholders are available.
    // Placeholders include: @current, @remaining, @total and @percentage
    'progress_message' => t('Import progress: Completed @current of @total stages.'),
    // The message that will be displayed to the end user if the batch fails.
    'error_message' => t('The Taxonomy Menu rebuild process encountered an error.'),
    'redirect' => 'admin/structure/taxonomy',
  );
  batch_set($batch);
  batch_process();
}

/**
 * Batch operation callback function: inserts 10 menu link items.
 *
 * @param array $terms
 *   Taxonomy terms as from taxonomy_get_tree().
 * @param string $menu_name
 *   Setting for the menu item name assigned to the vocabulary.
 * @param array $context
 *   Batch context array.
 *
 * @see _taxonomy_menu_insert_link_items_batch().
 */
function _taxonomy_menu_insert_link_items_process($terms, $menu_name, &$context) {
  _taxonomy_menu_batch_init_context($context, $start, $end, 10);

  // Loop through $terms to process each term.
  for ($i=$start; $i<count($terms) && $i<$end; $i++) {
    $args = array(
      'term' => $terms[$i],
      'menu_name' => $menu_name,
    );
    $mlid = taxonomy_menu_handler('insert', $args);
  }

  _taxonomy_menu_batch_update_context($context, $end, count($terms), 'Creating Menu Items');
}



/**
 * Batch finished callback: sets a message stating the menu has been updated.
 *
 * @see _taxonomy_menu_insert_link_items_batch().
 */
function _taxonomy_menu_insert_link_items_success() {
  // TODO state menu name here.
  drupal_set_message(t('The Taxonomy Menu has been updated.'));
}

/**
 * Initialise the batch context.
 *
 * @param array $context
 *   Batch context array.
 * @param int $start
 *   The item to start on in this pass.
 * @param int $end
 *   The end item of this pass.
 * @param int $items
 *   The number of items to process in this pass.
 */
function _taxonomy_menu_batch_init_context(&$context, &$start, &$end, $items) {
  // Initialize sandbox the first time through.
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }

  $start = $context['sandbox']['progress'];
  $end = $start + $items;
}

/**
 * Update the batch context
 *
 * @param array $context
 *   Batch context array.
 * @param int $end
 *   The end point of the most recent pass.
 * @param int $total
 *   The total number of items to process in this batch.
 * @param str $msg
 *   Message for the progress bar.
 * @see _taxonomy_menu_insert_link_items_process().
 */
function _taxonomy_menu_batch_update_context(&$context, $end, $total, $msg) {
  if ($end > $total) {
    $context['finished'] = 1;
    return;
  }
  $context['message'] = "{$msg}: {$end} of {$total}";
  $context['sandbox']['progress'] = $end;
  $context['finished'] = $end/$total;
}
