<?php
// $Id$

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
        'node' => array('type' => 'node', 'label' => t('Node on which the transition was done'))
      ));
  }

  return $events;
}