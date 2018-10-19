<?php

/**
 * @file
 * API documentation for drafty.
 */

/**
 * Allow access to information attached to a revision before it is deleted.
 *
 * @param string $entity_type
 *   The entity type.
 * @param int $entity_id
 *   The entity ID.
 * @param int $revision_id
 *   The revision ID to be deleted.
 * @param int $replaced_by
 *   The new revision ID that is replacing the one that is about to be deleted.
 */
function hook_drafty_predelete_revision($entity_type, $entity_id, $revision_id, $replaced_by) {
  if ($entity_type == 'node') {
    // Preserve the revision timestamp.
    $revision = node_load($entity_id, $revision_id);
    db_update('node_revision')
      ->fields(array(
        'timestamp' => $revision->revision_timestamp,
      ))
      ->condition('vid', $replaced_by)
      ->execute();
  }
}
