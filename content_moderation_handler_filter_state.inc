<?php
// $Id$

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer
// $Id$

/**
 * @file
 *   Filter state handlers.
 */
class content_moderation_handler_filter_state extends views_handler_filter {
  function option_definition() {
    $options = parent::option_definition();
    $options['value'] = array('default' => NULL);
    return $options;
  }

  function value_form(&$form, &$form_state) {
    $result = db_query("select name FROM {content_moderation_states}");
    while ($item = db_fetch_array($result)) {
      $options[$item['name']] = $item['name'];
    }
    $form['value'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $this->options['state'],
      '#title' => t('State'),
      '#description' => t('Select a workflow state (content moderation).'),
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . $this->operator . " '%s'", $this->value);
  }
}