<?php

/**
 * @file
 * Module's administration functionality.
 */

/**
 * Settings form.
 */
function bibliofil_lists_admin_settings_form($form, &$form_state) {
  $form['bibliofil_lists_opac_url'] = [
    '#type' => 'textfield',
    '#title' => t('OPAC URL'),
    '#description' => t("URL of client's OPAC service. Example: <i>https://presentasjon.bib.no</i>"),
    '#default_value' => variable_get('bibliofil_lists_opac_url', ''),
  ];

  $form['#submit'][] = 'bibliofil_lists_admin_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Custom form submit.
 */
function bibliofil_lists_admin_settings_form_submit($form, &$form_state) {
  // Remove trailing slash from the url.
  $opac_url = $form_state['values']['bibliofil_lists_opac_url'];

  if (preg_match('/\/$/', $opac_url)) {
    $cleared_url = preg_replace('/\/$/', '', $opac_url);
    $form_state['values']['bibliofil_lists_opac_url'] = $cleared_url;
  }
}
