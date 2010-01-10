<?

/*
 * General form to change the state of a revision
 * Currently we only support changing from one step to the other
 */
function content_moderation_change_state_form($formt_state,$vid) {
  global $user;
  $nextstate = 'foo';
  $curstate = _content_moderation_current_state($vid);
  // Here we check if the transition from the current state to the new state
  // is even possible
  if(_content_moderation_precondition($user,$curstate,$nextstate,$vid) == false){
    return;
  }
  $node = node_load($nid, $vid);

  $form['user'] = array (
		'#type' => 'value',
		'#value' => $user
  );
  $form['node'] = array (
		'#type' => 'value',
		'#value' => $node
  );

  $form['oldstate'] = array (
		'#type' => 'value',
		'#defaul_value' => $curstate
  );
  $form['nextstate'] = array (
		'#type' => 'value',
		'#defaul_value' => array($nextstate)
  );
  $form['description'] = array (
		'type' => 'fieldset',
		'title' => t('Changing state of revision')
  );
  return $form;
}


function content_moderation_change_state_submit($node,$newstate,$comment) {
  global $user;
  // TODO: think about locking !!!
  $values= array (
		'nid' => $node->nid,
		'vid' => $node->vid,
		'vid' => $node->vid,
  );

  drupal_write_record('content_moderation_history', $values, $keys);
}

function _content_moderation_change_state_link($vid,$nid) {
  return l('edit',"moderation/$nid/$vid/change");
}


/*
 * Get all states possible
 */
function _content_moderation_states() {
  $result = db_query('select * from {content_moderation_states}');
  //TODO: cache this
  $states = array();
  while ($state = db_fetch_object($result)) {
    $states[] = $state;
  }
  return $states;
}

/*
 * Generally check, whether the node-type can me moderated at all
 * and whether the user is allowed to change into that state for that
 * node type.
 */
function _content_moderation_statechange_allowed($newstate,$nid,$vid) {
  // TODO get node type  by vid
  dsm($nid);
  $node = node_load($nid,$vid);
  $type = $node->type;
  // User must have write access to do anything
  if(module_exists('impressive_media_icombinded_access')) {
    if(impressive_media_icombined_access_accesscheck('update', $node) === false) {
      return false;
    }
  }
  else {
    if(node_access('upate', $node) === false) {
      return false;
    }

  }
  // Is this content even in moderatation?
  if( variable_get( "node_options_$type", array(0) ) == 0) {
    return false;
  }
  // Ok, then lets see if the user has access to change the state
  // from the current state to the new one, respecting the node type
  $curstate = _content_moderation_current_state($vid);
  return user_access("content moderation $type state $curstate to $newstate");
}

/*
 * Get the current state of a revision
 *
 */
function _content_moderation_current_state($vid) {
  $result = db_query('select state from {content_moderation_revision_state} where vid=%d',$vid);
  $row = db_fetch_object($result);
  return $row->name;
}

?>
