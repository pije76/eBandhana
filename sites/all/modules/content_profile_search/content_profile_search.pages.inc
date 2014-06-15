<?php
/**
 * @file
 * Displays the search form to the user.
 */

/**
 * Content profile search form page.
 */
function content_profile_search_page() {
  $output  = drupal_get_form('content_profile_search_form');
  $output .= drupal_get_form('content_profile_search_results');
  return $output;
}

/**
 * Form builder; Return content profile search form.
 */
function content_profile_search_form() {
  $session = &$_SESSION['content_profile_search_filter'];
  $session = is_array($session) ? $session : array();

  // Create a list of field names and labels the current user may access, which
  // we can feed into the form as an option list.
  $fields = _content_profile_search_fields(TRUE);
  $options = _content_profile_search_fields_options($fields, TRUE);

  $form['search'] = array(
    '#type' => 'fieldset',
  );

  $form['search']['string'] = array(
    '#type' => 'textfield',
    '#title' => 'Search',
    '#description' => t("Enter arbitrary text to search for in all checked Content Profile fields and the user's username and email address."),
    '#default_value' => isset($session['string']) ? $session['string'] : '',
  );

  $form['search']['boolean'] = array(
    '#type' => 'radios',
    '#description' => t('Boolean search option.'),
    '#options' => array(
      'OR' => t('OR: At least one of the searched fields must contain one of the search terms.'),
      'AND' => t('AND: All of the searched fields must contain at least one of the search terms.'),
    ),
    '#default_value' => isset($session['boolean']) ? 'OR' : $session['boolean'],
  );

  $form['search']['filters'] = array(
    '#type' => 'fieldset',
    '#title' => 'Filters',
    '#description' => '',
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('content_profile_search_filter_collapse', 0) == 1) ? TRUE : FALSE,
  );

  $form['search']['filters']['fields'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Search fields',
    '#description' => t('Check all the fields that you wish to search. The login name and email address fields are always searched.'),
    '#options' => $options,
    '#default_value' => isset($session['fields']) ? $session['fields'] : variable_get('content_profile_search_default_fields', array()),
  );

  // Do not display the roles filter if there are no roles to filter by.
  $roles = _content_profile_search_roles();
  if (!empty($roles)) {
    $form['search']['filters']['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => 'User roles',
      '#description' => t('Check any roles you wish to limit results by.'),
      '#options' => $roles,
      '#default_value' => isset($session['roles']) ? $session['roles'] : variable_get('content_profile_search_default_roles', array()),
    );
  }

  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('List Users'),
  );
  return $form;
}

/**
 * Process results of the content_profile_search filter form.
 */
function content_profile_search_form_submit($form, &$form_state) {
  $_SESSION['content_profile_search_filter'] = $form_state['values'];
  $form_state['redirect'] = 'admin/user/user/content_profile_search';
}

/**
 * Form builder; 
 */
function content_profile_search_results() {
  $result = content_profile_search_build_query();

  $form['pager_top'] = array(
    '#type' => 'markup',
    '#value' => $result['pager'],
  );
  $form['results'] = array(
    '#type' => 'markup',
    '#value' => $result['data'],
    '#empty' => t('No people found.'),
  );
  $form['pager_bottom'] = array(
    '#type' => 'markup',
    '#value' => $result['pager'],
  );
  return $form;
}
