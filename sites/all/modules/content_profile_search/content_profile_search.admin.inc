<?php
/**
 * @file
 * Settings page callback for the content_profile_search module.
 */

/**
 * Display the settings form.
 */
function content_profile_search_admin_settings() {
  $form = array();

  // The admin interface lists all fields, but search only processes those the user may view.
  $fields = _content_profile_search_fields(FALSE);
  $options = _content_profile_search_fields_options($fields, TRUE);

  $form['content_profile_search_defaults'] = array(
    '#title' => t('Defaults'),
    '#description' => t('Set defaults for the user and profile search form.'),
    '#type' => 'fieldset',
  );

  $form['content_profile_search_defaults']['content_profile_search_filter_collapse'] = array(
    '#title' => t('Collapse Filters'),
    '#description' => t('Should the filters field be collapsed on the search page.'),
    '#type' => 'radios',
    '#options' => array(
      0 => t('Not collapsed'),
      1 => t('Collapsed'),
    ),
    '#default_value' => variable_get('content_profile_search_filter_collapse', 0),
  );

  if (!empty($fields)) {
    $form['content_profile_search_defaults']['content_profile_search_default_fields'] = array(
      '#title' => t('Default Fields'),
      '#description' => t('Select any fields that you want searched by default. The login name and email address fields are always searched.'),
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => variable_get('content_profile_search_default_fields', array()),
    );

    $roles = _content_profile_search_roles();
    if (!empty($roles)) {
      $form['content_profile_search_defaults']['content_profile_search_default_roles'] = array(
        '#title' => t('Default Roles'),
        '#description' => t('Select any roles you want limit results to.'),
        '#type' => 'checkboxes',
        '#options' => $roles,
        '#default_value' => variable_get('content_profile_search_default_roles', array()),
      );
    }
    else {
      $form['content_profile_search_defaults']['content_profile_search_no_custom_roles'] = array(
        '#title' => t('Default Roles'),
        '#value' => t('No custom roles have been defined. Authenticated users searched by default.'),
        '#type' => 'markup',
      );
    }
    $form['content_profile_search_defaults']['content_profile_pager_items'] = array(
      '#title' => t('Results per page'),
      '#type' => 'textfield',
      '#size' => 4,
      '#description' => t('The number of search results that will be shown per page. Setting this too high may affect site performance.'),
      '#default_value' => variable_get('content_profile_pager_items', 50),
    );
  }
  else {
    drupal_set_message(t('You have not yet defined any content profile text fields.'), 'error');
  }

  return system_settings_form($form);
}

function content_profile_search_admin_settings_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['content_profile_pager_items'])) {
    form_set_error('content_profile_pager_items', t('You need to enter a positive number greater than 0'));
  }
  else if (intval($form_state['values']['content_profile_pager_items']) <= 0) {
    form_set_error('content_profile_pager_items', t('You need to enter a positive number greater than 0'));
  } else if (intval($form_state['values']['content_profile_pager_items']) >= 1000) {
    drupal_set_message(t('Setting results per page too high may affect site performance'), 'warning');
  }
}
