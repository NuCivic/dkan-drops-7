<?php

/**
 * @file
 * Default template for a Fieldable Panels Panes entity.
 */

/**
 * Available variables.
 *
 * $classes - String containing all of the classes defined for this object.
 * $attributes - String containing additional HTML attributes to be displayed.
 * $title_suffix - Render array.
 * $content - Render array containing all field data to be output.
 */
?>
<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_suffix); ?>
  <?php print render($content); ?>
</div>
