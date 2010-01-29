<?php
// $Id$

// For docs have a look on http://drupal.org/node/375833

/**
 * Implementation of hook_hook_info().
 * For every node type moderation is activated for and every transition we generate a event
 */
function content_moderation_hook_info() {
  $triggers['content_moderation']['content_moderation'] =  array();
  $matrix = _content_moderation_node_transition_matrix();
  foreach($matrix as $op => $descr) {
    $triggers['content_moderation']['content_moderation'][$op]['runs when'] = $descr;
  }

  return $triggers;
}

/*
 * Implementation of hook_trigger_name().
 */
function content_moderation_content_moderation($op,$user) {
  if (!module_exists("trigger")) {
    return;
  }

  $matrix = _content_moderation_node_transition_matrix();
  if(!in_array($op,array_keys($matrix))) {
    return;
  }

  $aids = _trigger_get_hook_aids('content_moderation', $op);
  if (count($aids) == 0) {
    return;
  }
  $context = array('hook' => 'content_moderation', 'op' => $op, 'user' => $user);

  actions_do(array_keys($aids), $user, $context);
}
