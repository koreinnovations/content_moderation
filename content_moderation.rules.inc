<?php

/*
 * Implementation of the rules hook_rules_event_info
 * Docs at: http://drupal.org/node/298549
 */
function content_moderation_rules_event_info() {
  $events = array();
  $matrix = _content_moderation_node_transition_matrix();
  foreach($matrix as $op => $descr) {
    $events[$op] = array(
      'label' => $descr,
      'module' => 'Content_moderation',
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Node on which the transition was done')),
	'author' => array('type' => 'user', 'label' => t('Author of the node'),'handler' => 'rules_events_argument_node_author'),
	'user' => array('type' => 'user', 'label' => t('The current user'), 'handler' => 'rules_events_global_user_argument'),
      ));
  }

  return $events;
}