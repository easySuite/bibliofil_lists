<?php

/**
 * @file
 * Suggestion lists custom panel pane.
 */

$plugin = [
  'single' => TRUE,
  'title' => t('Bibliofil lists: Suggestions'),
  'description' => t('Displays lists with Bibliofil suggested materials'),
  'category' => t('Bibliofil'),
  'edit form' => 'bibliofil_lists_suggestions_edit_form',
  'render callback' => 'bibliofil_lists_suggestions_render',
  'defaults' => [
    'toplister' => 1,
    'ants' => 1,
    'ante' => 1,
    'instans' => 'm2-cms',
  ],
];

/**
 * Return pane contents.
 *
 * @throws \Exception
 */
function bibliofil_lists_suggestions_render($subtype, $conf, $panel_args, $context) {
  $carousel_wrapper = [
    '#type' => 'container',
    '#attributes' => [
      'id' => 'bibliofil-lists',
      'data-bibliofil-lists-type' => 'suggestions',
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
function bibliofil_lists_suggestions_edit_form($form, $form_state) {
  $conf = $form_state['conf'];

  $options = [
    'sjangre' => 'sjangre',
    'emner' => 'emner',
    'kikkhull' => 'kikkhull',
  ];

  $form['list_type'] = [
    '#type' => 'select',
    '#title' => t('List type'),
    '#description' => t('Select the type of lists to be displayed. Leave empty for all types.'),
    '#options' => $options,
    '#default_value' => $conf['list_type'],
    '#empty_option' => t('Select'),
  ];

  $form['settings'] = [
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  ];

  $form['settings']['toplister'] = [
    '#type' => 'checkbox',
    '#title' => t('Expose popular items lists (toplister)'),
    '#default_value' => $conf['toplister'],
  ];

  $form['settings']['ants'] = [
    '#type' => 'textfield',
    '#title' => t('Amount of lists by genres (ants)'),
    '#description' => t('Represents the amount of lists exposed with genres category.'),
    '#default_value' => $conf['ants'],
  ];

  $form['settings']['ante'] = [
    '#type' => 'textfield',
    '#title' => t('Amount of lists by topics (ante)'),
    '#description' => t('Represents the amount of lists exposed with topics category.'),
    '#default_value' => $conf['ante'],
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
function bibliofil_lists_suggestions_edit_form_submit($form, &$form_state) {
  $form_state['conf']['list_type'] = $form_state['values']['list_type'];

  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}
