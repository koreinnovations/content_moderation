<?

/*
 * General form to change the state of a revision
 * Currently we only support changing from one step to the other
 */
function content_moderation_change_state_form(&$form_state,$vid) {
  global $user;

  $curstate = _content_moderation_current_state($vid) ;
  $nid = _content_moderation_get_nid($vid);
  $node = node_load($nid, $vid);
  // Get all states we can switch to respecting the user rights and the transition
  // table
  $states = _content_moderation_next_states($curstate, $node->type, $user);

  $form = array();
  $form['#attributes'] = array('enctype'=>"multipart/form-data");
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
    '#default_value' => $curstate
  );

  $form['description'] = array (
    '#type' => 'fieldset',
    '#title' => t('Changing state of revision'),
  );

  $form['description']['nextstate'] = array (
    '#type' => 'select',
    '#title' => t('New state'),
    '#prefix' => t('Current state: @curstate',array('@curstate' => $curstate)),
    '#description' => t('Choose the next workflow state this revision should be set to.'),
    '#options' => $states,
  );

  $form['description']['comment'] = array (
    '#type' => 'textarea',
    '#title' => t('Comment'),
    '#description' => t('If you wish, add an optional comment. This will be shown in the history')
  );

  $form['buttons']['submit']            = array ('#type'=>'submit', '#value'=>t('Save'));
  $form['buttons']['cancel']['#type']   = 'markup';
  $form['buttons']['cancel']['#weight'] = 2000;
  $form['buttons']['cancel']['#value']  = l(t('Cancel'), "node/$nid/revisions/$vid/view", array( 'attributes' => array( 'class' => 'form-submit form-submit-cancel')));
  return $form;
}

/*
 * Implementation of hook_form_validate
 * Validate if the user is providing a valid next state and is allowed to change state
 * of the provided node
 */
function content_moderation_change_state_form_validate($form_id, $form_values) {
  global $user;

  // Is this user allowed to change the state of this revision?
  if(!_content_moderation_statechange_allowed($form_values['values']['node']->vid, $form_values['values']['nextstate'])) {
      form_set_error('nextstate',t('You are not allowed to change to that state!'));
  }
}

/*
 * Implementation of hook_form_submit
 */
function content_moderation_change_state_form_submit($form, &$form_state) {
  global $user;

  $node = $form_state['values']['node'];
  $curstate = $form_state['values']['oldstate'];
  $nextstate = $form_state['values']['nextstate'];
  $comment = $form_state['values']['comment'];

  // Change the revision state.
  _content_moderation_update_revision_state($node->vid,$node->nid,$nextstate);

  // If the state is live, change the current live version.
  // TODO: the "final" state maybe should not be hardcoded later.
  if($nextstate == 'live') {
    _content_moderation_set_live($node->vid,$node->nid);
    drupal_set_message(t("This revision {$node->vid} has now been published as the new live version."));
  }

  // History entry.
  _content_moderation_save_history($node,$curstate,$nextstate,$user->uid,$comment);
  drupal_set_message(t('The revisions state has been successfully updated to @state',array('@state' => $nextstate)));

  if($nextstate == 'live') {
   drupal_goto(url('node/'.$node->nid));
  }
  else {
    drupal_goto(url("node/{$node->nid}/revisions/{$node->vid}/view"));
  }
}

/*
 * Update internal live-version entry.
 */
function _content_moderation_update_revision_state($vid,$nid,$state) {
  $revison_state = array(
    'nid' => $nid,
    'vid' => $vid,
    'state' => $state,
  );
  $keys = array();
  // Check if we have an entry allready. Update if needed.
  $result = db_fetch_object( db_query("select vid from {content_moderation_revision_state} where vid=%d", $vid) );
  if(is_object($result)) {
    $keys = array('vid');
  }
  drupal_write_record('content_moderation_revision_state', $revison_state, $keys);
}


/*
 * Make the given vid the new live version.
 */
function _content_moderation_set_live($vid,$nid) {
  // Save the vid in the node table.
  db_query('UPDATE {node} SET vid = %d WHERE nid = %d', $vid, $nid);
  // Reset all current states of revisisions of this node, which are live
  // as we can have more then one live revision.
  // We are not resetting the history though, because it should be visible
  // which node has been live, approved by who and so forth.
  db_query('UPDATE {content_moderation_revision_state} SET state = "none" WHERE nid = %d AND state="live"', $nid);
}

/*
 * Save an entry in the content moderation history
 */
function _content_moderation_save_history($node,$curstate,$nextstate,$uid,$comment) {
   $history = array (
    'nid' => $node->nid,
    'vid' => $node->vid,
    'uid' => $uid,
    'old_state_name' => $curstate,
    'state_name' => $nextstate,
    'stamp' => time(),
    'comment' => $comment
  );

  drupal_write_record('content_moderation_node_history', $history, array());
}

/*
 * Gets the history of a node or a revision, depending on the key.
 */
function _content_moderation_get_history($key) {
  $result = db_query('select * from {content_moderation_node_history} where '.$key['name'].'=%d order by stamp DESC' ,$key['value']);
  $history = array();

  while ($entry = db_fetch_object($result)) {
    $history[] = $entry;
  }

  return $history;
}

/*
 * Checks all transition from the current revision state to any valid other state.
 * Optional also filters any next-states which this user is not allowed to change to
 *
 * @curstate: current state of the revision
 * @node_type: the node type is used to look for valid transition for this specific node type
 * @user: if set, all resulting states are filter to states allowed for this user
 */
function _content_moderation_next_states($curstate, $node_type = 'all', $user = NULL) {
  if($curstate == "")
    $curstate = 'none';
  $result = db_query('select to_name as state from {content_moderation_transition} where from_name="%s" and (ntype="%s" OR ntype="all")',$curstate, $node_type);
  $states = array();
  while ($state = db_fetch_object($result)) {
    $states[$state->state] = t($state->state);
  }

  if($user != NULL) {
    foreach($states as $state_name => $value) {
      if(!_content_moderation_state_allowed($user, $curstate, $state_name, $node_type)) {
        unset($states[$state_name]);
      }
    }
  }
  return $states;
}

/*
 * Get all states possible
 */
function _content_moderation_states() {
  $result = db_query('select * from {content_moderation_states}');
  //TODO: cache this
  $states = array();
  while ($state = db_fetch_object($result)) {
    $states[] = $state->name;
  }
  return $states;
}

/*
 * Generally check, whether the node-type can be moderated at all
 * and whether the user is allowed to change into that state for that
 * node type.
 */
function _content_moderation_statechange_allowed($vid,$state = NULL) {
  global $user;
  $nid = _content_moderation_get_nid($vid);
  $node = node_load($nid,$vid);

  // Is this content even in moderatation?
  if(!_content_moderation_moderate_node_type($node->type)) {
    drupal_set_message(t('this content type ('.$node->type.') is not under moderation'),'error');
    return false;
  }

  // User must have write access to do anything state related
  if(module_exists('impressive_media_icombinded_access')) {
    if(impressive_media_icombined_access_accesscheck('update', $node) === false) {
      return false;
    }
  }
  else {
    if(node_access('update', $node) === false) {
      return false;
    }
  }

  // Does the user has rights to change to any allowed next states?
  $curstate = _content_moderation_current_state($vid);
  if($state != NULL) {
    return _content_moderation_state_allowed($user, $curstate, $state, $node->type);
  }
  else if(count(_content_moderation_next_states($curstate, $node->type, $user)) > 0) {
    return true;
  }
  //else
  return false;
}

/*
 * Get the current state of a revision *
 */
function _content_moderation_current_state($vid) {
  $result = db_query('select state from {content_moderation_revision_state} where vid=%d',$vid);
  $row = db_fetch_object($result);

  if(is_object($row)) {
    return $row->state;
  }

  // return default
  return 'none';
}