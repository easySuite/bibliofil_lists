<?php

/**
 * @file
 * Library lists custom panel pane.
 */

$plugin = [
  'single' => TRUE,
  'title' => t('Bibliofil lists: Library lists'),
  'description' => t('Displays lists with Bibliofil suggested materials from existing lists'),
  'category' => t('Bibliofil'),
  'edit form' => 'bibliofil_lists_librarylists_edit_form',
  'render callback' => 'bibliofil_lists_librarylists_render',
  'defaults' => [
    'random' => 1,
    'instans' => 'm2-cms',
  ],
];

/**
 * Return pane contents.
 *
 * @throws \Exception
 */
function bibliofil_lists_librarylists_render($subtype, $conf, $panel_args, $context) {
  $carousel_wrapper = [
    '#type' => 'container',
    '#attributes' => [
      'id' => 'bibliofil-lists',
      'data-bibliofil-lists-type' => 'librarylists',
      'data-bibliofil-lists-hash' => drupal_base64_encode(implode(':', $conf)),
    ],
    '#attached' => [
      'js' => [
        drupal_get_path('module', 'bibliofil_lists') . '/js/bibliofil_lists.js',
        drupal_get_path('module', 'ding_carousel') . '/js/ding_carousel.js',
      ],
    ],
  ];

  $block = new stdClass();
  $block->title = t('Bibliofil list');
  $block->content = $carousel_wrapper;

  return $block;
}

/**
 * Panel configuration form.
 */
function bibliofil_lists_librarylists_edit_form($form, $form_state) {
  if (empty(variable_get('bibliofil_lists_opac_url', ''))) {
    drupal_set_message(t('Module is not configured. Please visit configuration page <a href="@link">admin/config/bibliofil/bibliofil_lists</a>.', ['@link' => url('/admin/config/bibliofil/bibliofil_lists', ['absolute' => TRUE])]), 'warning');
    return $form;
  }

  $conf = $form_state['conf'];

  $form['settings'] = [
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  ];

  $form['settings']['random'] = [
    '#type' => 'checkbox',
    '#title' => t('Show random items (random)'),
    '#description' => t('Show random items from random library list.'),
    '#default_value' => $conf['random'],
  ];

  $form['settings']['instans'] = [
    '#type' => 'textfield',
    '#title' => t('Instance (instans)'),
    '#description' => t('Indicates which instance of the m2 the links should go to.'),
    '#default_value' => $conf['instans'],
  ];

  return $form;
}

/**
 * Panel configuration form submit.
 */
function bibliofil_lists_librarylists_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}
