<?php

/**
 * @file
 * Autoload API.
 */

/**
 * Allow modules to add/remove definitions.
 *
 * In general, this hook ought to be used for providing autoloading for
 * external dependencies.
 *
 * @param \AutoloadCache $autoload
 *   The autoloading cache.
 *
 * @see \AutoloadCache::rebuild()
 */
function hook_autoload_lookup_alter(\AutoloadCache $autoload) {

}
