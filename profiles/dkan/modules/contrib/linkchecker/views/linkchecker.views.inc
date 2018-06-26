<?php

/**
 * @file
 * Views callbacks for the linkchecker module.
 */

/**
 * Implements hook_views_data().
 */
function linkchecker_views_data() {
  $data['linkchecker_link'] = array(
    'table' => array(
      'group' => t('Broken links'),
      // Define this as a base table.
      'base' => array(
        'field' => 'lid',
        'title' => t('Broken links'),
        'help' => t('Broken links related to nodes, blocks, comments, fields, etc.'),
      ),
      // Define joins.
      'join' => array(
        'linkchecker_block_custom' => array(
          'left_field' => 'lid',
          'field' => 'lid',
        ),
        'linkchecker_comment' => array(
          'left_field' => 'lid',
          'field' => 'lid',
        ),
        'linkchecker_node' => array(
          'left_field' => 'lid',
          'field' => 'lid',
        ),
      ),
    ),
    // Describe table fields to views.
    'lid' => array(
      'title' => t('Link ID'),
      'help' => t("The unique linkchecker link ID."),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'url' => array(
      'title' => t('URL'),
      'help' => t('The URL of a link in a broken links report.'),
      'field' => array(
        'handler' => 'linkchecker_handler_field_linkchecker_link_url',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    // HTTP status code.
    'code' => array(
      'title' => t('HTTP status code'),
      'help' => t('The status code returned by HTTP requests to the link.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'linkchecker_handler_filter_linkchecker_link_code',
      ),
    ),
    // Error message.
    'error' => array(
      'title' => t('Error message'),
      'help' => t('The error message received from the remote server while doing link checking.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    // Fail count.
    'fail_count' => array(
      'title' => t('Fail count'),
      'help' => t('The number of times a request for the URL has failed.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    // Last checked unix timestamp.
    'last_checked' => array(
      'title' => t('Last checked'),
      'help' => t('Unix timestamp of the last link check.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    // Status (TRUE|FALSE if the link check is enabled|disabled).
    'status' => array(
      'title' => t('Status'),
      'help' => t('Whether or not the link should be checked for being broken.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'label' => t('Check link status'),
        'type' => 'enabled-disabled',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    // Link to edit the link settings.
    'link_edit' => array(
      'field' => array(
        'title' => t('Edit link settings'),
        'help' => t('Provide a simple link to edit the link settings.'),
        'handler' => 'linkchecker_handler_field_linkchecker_link_link_edit',
      ),
    ),
  );

  $data['linkchecker_node'] = array(
    'table' => array(
      'group' => t('Broken links'),
      'join' => array(
        'linkchecker_link' => array(
          'left_field' => 'lid',
          'field' => 'lid',
        ),
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
        ),
      ),
    ),
    // Describe table fields to views.
    'lid' => array(
      'title' => t('Link ID'),
      'help' => t("The unique linkchecker link ID."),
    ),
    'nid' => array(
      'title' => t('Content with link'),
      'help' => t('Relate all content associted with a link.'),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'node',
        'base field' => 'nid',
        'label' => t('Node'),
        'skip base' => 'node',
      ),
    ),
  );

  if (module_exists('comment')) {
    $data['linkchecker_comment'] = array(
      'table' => array(
        'group' => t('Broken links'),
        'join' => array(
          'linkchecker_link' => array(
            'left_field' => 'lid',
            'field' => 'lid',
          ),
          'comment' => array(
            'left_field' => 'cid',
            'field' => 'cid',
          ),
        ),
      ),
      // Describe table fields to views.
      'lid' => array(
        'title' => t('Link ID'),
        'help' => t("The unique linkchecker link ID."),
      ),
      'cid' => array(
        'title' => t('Comment with link'),
        'help' => t('Relate all comments associted with a link.'),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'comment',
          'base field' => 'cid',
          'label' => t('Comment'),
          'skip base' => 'comment',
        ),
      ),
    );
  }

  if (module_exists('block')) {
    $data['linkchecker_block_custom'] = array(
      'table' => array(
        'group' => t('Broken links'),
        'join' => array(
          'linkchecker_link' => array(
            'left_field' => 'lid',
            'field' => 'lid',
          ),
        ),
      ),
      // Describe table fields to views.
      'lid' => array(
        'title' => t('Link ID'),
        'help' => t('The unique linkchecker link ID.'),
      ),
      'bid' => array(
        'title' => t('Block ID'),
        'help' => t('The unique block ID.'),
        'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
      ),
      // Link to edit the block.
      'block_edit' => array(
        'field' => array(
          'title' => t('Edit block'),
          'help' => t('Provide a simple link to edit the block.'),
          'handler' => 'linkchecker_handler_field_linkchecker_block_custom_block_edit',
        ),
      ),
    );
  }

  // Add a 'Redirect add' link if the Redirect module is installed.
  if (module_exists('redirect')) {
    $data['linkchecker_link']['redirect_add'] = array(
      'field' => array(
        'title' => t('Create redirect'),
        'help' => t('Provide a simple link to create a redirect.'),
        'handler' => 'linkchecker_handler_field_linkchecker_link_redirect_add',
      ),
    );
  }

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function linkchecker_views_plugins() {
  return array(
    'access' => array(
      'linkchecker' => array(
        'title' => t('Linkchecker'),
        'handler' => 'linkchecker_plugin_access',
      ),
    ),
  );
}

/**
 * Implements hook_views_pre_render().
 */
function linkchecker_views_pre_render(&$view) {
  // If the view's base table is linkchecker_link and it's a page display.
  if ($view->base_table == 'linkchecker_link' && $view->display[$view->current_display]->display_plugin == 'page') {
    // Set a status message if any unchecked links exist.
    $links_unchecked = db_query('SELECT COUNT(1) FROM {linkchecker_link} WHERE last_checked = :last_checked AND status = :status', array(':last_checked' => 0, ':status' => 1))->fetchField();
    if ($links_unchecked > 0) {
      $links_all = db_query('SELECT COUNT(1) FROM {linkchecker_link} WHERE status = :status', array(':status' => 1))->fetchField();
      drupal_set_message(format_plural($links_unchecked,
        'There is 1 unchecked link of about @links_all links in the database. Please be patient until all links have been checked via cron.',
        'There are @count unchecked links of about @links_all links in the database. Please be patient until all links have been checked via cron.',
        array('@links_all' => $links_all)), 'warning');
    }
  }
}
