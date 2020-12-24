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
  $block = new stdClass();
  $block->title = t('Bibliofil list');

  $opac_url = variable_get('bibliofil_lists_opac_url', '');

  $path = '/cgi-bin/m2dyn.htmc';

  $query_params = [
    'mode' => 'hentupforslag',
    'json' => 1,
    'toplister' => $conf['toplister'],
    'ants' => $conf['ants'],
    'ante' => $conf['ante'],
    'instans' => $conf['instans'],
  ];

  $url = url($opac_url . $path, ['query' => $query_params]);

  $result = drupal_http_request($url);

  if ($result->code != 200) {
    watchdog('bibliofil_lists', 'There was problem on requesting API.', NULL, WATCHDOG_ERROR);
  }

  $data = drupal_json_decode($result->data);

  $tabs = [];
  if (!empty($conf['list_type'])) {
    if (!empty($data[$conf['list_type']])) {
      foreach ($data[$conf['list_type']] as $key => $item) {
        $tabs[$key]['#title'] = $item['name'];
        $tabs[$key]['#theme'] = 'ding_carousel';
        $tabs[$key]['#placeholders'] = '';
        $tabs[$key]['#offset'] = 0;
        $tabs[$key]['#path'] = '';

        foreach ($item['titler'] as $k => $i) {
          $tabs[$key]['#items'][] = [
            '#markup' => theme('bibliofil_lists_item', [
              'type' => 'suggestions',
              'opac_url' => $opac_url,
              'data' => $i,
            ]),
          ];
        }
      }
    }
  }
  else {
    $nonempty = array_filter($data, function ($category) {
      if (is_array($category)) {
        return $category;
      }
    });

    $inlined = [];
    foreach ($nonempty as $type) {
      foreach ($type as $t) {
        $inlined[] = $t;
      }
    }

    foreach ($inlined as $key => $tabgen) {
      $tabs[$key]['#title'] = $tabgen['name'];
      $tabs[$key]['#theme'] = 'ding_carousel';
      $tabs[$key]['#placeholders'] = '';
      $tabs[$key]['#offset'] = 0;
      $tabs[$key]['#path'] = NULL;

      foreach ($tabgen['titler'] as $k => $i) {
        $tabs[$key]['#items'][] = [
          '#markup' => theme('bibliofil_lists_item', [
            'type' => 'suggestions',
            'opac_url' => $opac_url,
            'data' => $i,
          ]),
        ];
      }
    }
  }

  $element = [
    '#theme' => 'ding_tabbed_carousel',
    '#transition' => 'fade',
    '#tabs' => $tabs,
    '#attached' => [
      'js' => [drupal_get_path('module', 'ding_carousel') . '/js/ding_carousel.js'],
    ],
  ];

  $block->content = $element;

  return $block;
}

/**
 * Panel configuration form.
 */
function bibliofil_lists_suggestions_edit_form($form, $form_state) {
  if (empty(variable_get('bibliofil_lists_opac_url', ''))) {
    drupal_set_message(t('Module is not configured. Please visit configuration page <a href="@link">admin/config/bibliofil/bibliofil_lists</a>.', ['@link' => url('/admin/config/bibliofil/bibliofil_lists', ['absolute' => TRUE])]), 'warning');
    return FALSE;
  }

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
    '#title' => t('Amount of lists by topics (ants)'),
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
