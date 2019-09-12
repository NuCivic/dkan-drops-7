<?php

/**
 * @file
 * Contains callbacks for service resource manipulation.
 */

/**
 * Implements hook_services_resources().
 */
function fieldable_panels_panes_services_resources() {
  return array(
    'fieldable_panels_panes' => array(
      'operations' => array(
        'retrieve' => array(
          'help' => t('This method returns a fieldable panels pane.'),
          'callback' => 'fieldable_panels_panes_services_retrieve',
          'file' => array(
            'type' => 'inc',
            'module' => 'fieldable_panels_panes',
            'name' => 'fieldable_panels_panes.services',
          ),
          'access callback' => 'fieldable_panels_panes_services_access',
          'access arguments' => array('view'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name' => 'fpid',
              'type' => 'int',
              'description' => t('The id of the fieldable panels pane to get.'),
              'source' => array('path' => '0'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'vid',
              'type' => 'int',
              'description' => t('The vid of the fieldable panels pane to get.'),
              'source' => array('param' => 'vid'),
              'optional' => TRUE,
            ),
          ),
        ),
        'create' => array(
          'help' => t('This method creates a fieldable panels pane.'),
          'callback' => 'fieldable_panels_panes_services_create',
          'file' => array(
            'type' => 'inc',
            'module' => 'fieldable_panels_panes',
            'name' => 'fieldable_panels_panes.services',
          ),
          'access callback' => 'fieldable_panels_panes_services_access',
          'access arguments' => array('create'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name' => 'data',
              'type' => 'struct',
              'description' => t('An object representing the fieldable panels pane.'),
              'source' => 'data',
              'optional' => FALSE,
            ),
          ),
        ),
        'update' => array(
          'help' => t('This method updates a fieldable panels pane.'),
          'callback' => 'fieldable_panels_panes_services_update',
          'file' => array(
            'type' => 'inc',
            'module' => 'fieldable_panels_panes',
            'name' => 'fieldable_panels_panes.services',
          ),
          'access callback' => 'fieldable_panels_panes_services_access',
          'access arguments' => array('update'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name' => 'fpid',
              'type' => 'int',
              'description' => t('The id of the fieldable panels pane to update.'),
              'source' => array('path' => '0'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'data',
              'type' => 'struct',
              'description' => t('An object representing the fieldable panels pane.'),
              'source' => 'data',
              'optional' => FALSE,
            ),
          ),
        ),
        'delete' => array(
          'help' => t('This method deletes a fieldable panels pane.'),
          'callback' => 'fieldable_panels_panes_services_delete',
          'file' => array(
            'type' => 'inc',
            'module' => 'fieldable_panels_panes',
            'name' => 'fieldable_panels_panes.services',
          ),
          'access callback' => 'fieldable_panels_panes_services_access',
          'access arguments' => array('delete'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name' => 'fpid',
              'type' => 'int',
              'description' => t('The id of the fieldable panels pane to delete.'),
              'source' => array('path' => '0'),
              'optional' => FALSE,
            ),
          ),
        ),
        'index' => array(
          'help' => t('This method returns a listing of fieldable panels panes.'),
          'callback' => 'fieldable_panels_panes_services_index',
          'file' => array(
            'type' => 'inc',
            'module' => 'fieldable_panels_panes',
            'name' => 'fieldable_panels_panes.services',
          ),
          'access arguments' => array('administer fieldable panels panes'),
          'args' => array(
            array(
              'name' => 'page',
              'optional' => TRUE,
              'type' => 'int',
              'description' => t('The zero-based index of the page to get, defaults to 0.'),
              'default value' => 0,
              'source' => array('param' => 'page'),
            ),
            array(
              'name' => 'fields',
              'optional' => TRUE,
              'type' => 'string',
              'description' => t('The fields to return.'),
              'default value' => '*',
              'source' => array('param' => 'fields'),
            ),
            array(
              'name' => 'parameters',
              'optional' => TRUE,
              'type' => 'array',
              'description' => t('Fields an values to filter the list by.'),
              'default value' => array(),
              'source' => array('param' => 'parameters'),
            ),
            array(
              'name' => 'pagesize',
              'optional' => TRUE,
              'type' => 'int',
              'description' => t('Number of records to get per page.'),
              'default value' => 20,
              'source' => array('param' => 'pagesize'),
            ),
            array(
              'name' => 'options',
              'optional' => TRUE,
              'type' => 'array',
              'description' => 'Additional query options.',
              'default value' => array(
                'orderby' => array(
                  'created' => 'DESC',
                ),
              ),
              'source' => array('param' => 'options'),
            ),
          ),
        ),
      ),
    ),
  );
}

/**
 * Access callback for the fieldable panels pane resource.
 *
 * @param string $op
 *   The operation that's going to be performed.
 * @param array $args
 *   The arguments that will be passed to the callback.
 *
 * @return bool
 *   Whether access is given or not.
 */
function fieldable_panels_panes_services_access($op, array $args) {
  // Make sure we have an object or this all fails, some servers can
  // mess up the types.
  if (is_array($args[0])) {
    $args[0] = (object) $args[0];
  }
  // This is to determine if it is just a string happens on node/%NID.
  elseif (!is_array($args[0]) && !is_object($args[0])) {
    $args[0] = (object) array('fpid' => $args[0]);
  }

  if ($op != 'create' && !empty($args)) {
    $fpp = fieldable_panels_panes_load($args[0]->fpid);
  }
  elseif ($op == 'create') {
    if (isset($args[0]->bundle)) {
      $fpp = $args[0]->bundle;
      return fieldable_panels_panes_access($op, $fpp);
    }
    else {
      return services_error(t('Fieldable panels pane bundle is required'), 406);
    }
  }
  if (isset($fpp->fpid) && $fpp->fpid) {
    return fieldable_panels_panes_access($op, $fpp);
  }
  else {
    return services_error(t('Fieldable panel pane @fpid could not be found', array('@fpid' => $args[0]->fpid)), 404);
  }
}

/**
 * Returns a specified fieldable panels pane.
 *
 * @param int $fpid
 *   The Fieldable Panels Pane ID.
 * @param int $vid
 *   The Fieldable Panels Pane VID.
 *
 * @return bool|mixed
 *   Fieldable Panel Pane Entity.
 */
function fieldable_panels_panes_services_retrieve($fpid, $vid = NULL) {
  return fieldable_panels_panes_load($fpid, $vid);
}

/**
 * Adds a new Fieldable Panels Pane and return the fpid.
 *
 * @param array $fpp
 *   The data to create the fieldable panels pane with.
 *
 * @return int|mixed
 *   Returns the ID of the Fieldable Panels Pane or errors.
 */
function fieldable_panels_panes_services_create(array $fpp) {
  $fpp_new = fieldable_panels_panes_create($fpp);

  $form_state = array();
  $form_state['values'] = $fpp;
  $form_state['entity'] = $fpp_new;

  drupal_form_submit('fieldable_panels_panes_entity_edit_form', $form_state);

  if ($errors = form_get_errors()) {
    return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
  }

  $fpp = $form_state['entity'];

  return array(
    'fpid' => $fpp->fpid,
    'uri' => services_resource_uri(array('fieldable_panels_pane', $fpp->fpid)),
  );
}

/**
 * Updates a Fieldable Panels Pane.
 *
 * @param int $fpid
 *   The Fieldable Panels Pane ID.
 * @param array $fpp
 *   Fieldable Panels Pane.
 *
 * @return int|mixed
 *   Unique identifier for the Fieldable Panels Pane or FALSE if there was a
 *   problem.
 */
function fieldable_panels_panes_services_update($fpid, array $fpp) {
  $fpp['pfid'] = $fpid;

  $old_fpp = fieldable_panels_panes_load($fpid);
  if (empty($old_fpp)) {
    return services_error(t('Fieldable Panels Pane @fpid not found.', array('@fpid' => $fpid)), 404);
  }

  // Setup form_state.
  $form_state = array();
  $form_state['values'] = $fpp;
  $form_state['entity'] = $old_fpp;

  drupal_form_submit('fieldable_panels_panes_entity_edit_form', $form_state);

  if ($errors = form_get_errors()) {
    return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
  }

  return $fpid;
}

/**
 * Delete a fieldable panels pane.
 *
 * @param int $fpid
 *   Unique identifier of the fieldable panels pane to delete.
 *
 * @return bool|mixed
 *   TRUE if deleted successfully, or services_error().
 */
function fieldable_panels_panes_services_delete($fpid) {
  $fpp = fieldable_panels_panes_load($fpid);
  if (empty($fpp)) {
    return services_error(t('There is no fieldable panels pane found with id @fpid.', array('@fpid' => $fpid)), 404);
  }

  // Delete fieldable panels pane.
  fieldable_panels_panes_delete($fpid);

  return TRUE;
}

/**
 * Return an array of optionally paged bids based on a set of criteria.
 *
 * An example request might look like:
 *
 * http://domain/endpoint/fieldable_panels_panes?fields=fpid,label&parameters[bundle]=text
 *
 * This would return an array of objects with only pfid and label defined, where
 * bundle = 'text'.
 *
 * @param int $page
 *   Page number of results to return (in pages of 20).
 * @param string $fields
 *   The fields you want returned as a string separated by commas.
 * @param array $parameters
 *   Fields and values used to build a sql WHERE clause indicating items to
 *   retrieve.
 * @param int $page_size
 *   Integer number of items to be returned.
 *
 * @return array
 *   An array of fieldable_panels_pane objects.
 */
function fieldable_panels_panes_services_index($page, $fields, array $parameters, $page_size, $options = array()) {
  $fpp_select = db_select('fieldable_panels_panes', 't')->orderBy('bundle', 'ASC');

  services_resource_build_index_query($fpp_select, $page, $fields, $parameters, $page_size, 'fieldable_panels_pane', $options);

  $results = services_resource_execute_index_query($fpp_select);

  return services_resource_build_index_list($results, 'fieldable_panels_pane', 'fpid');
}
