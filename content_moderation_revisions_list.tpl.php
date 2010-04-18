<?php
// $Id$

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer
/*
 * Usable values
 * $node : currently viewed node
 * $state: workflow state of the currently viewed node revision
 * $live: live version of the node
 * $live_link: revision and link to the live node
 * $links: actions on the different revisions (compare_with_live,compare,delete_revision, edit_revision, edit_live)
 */

if(!is_array($revisions) || count($revisions) == 0) {
  return "";
}
?>
<div class='revision_list'>
<?php
$edit_icon = '<span class="tinyicon t_editicon"></span>';
$rev_icon = '<span class="tinyicon t_revisionicon"></span>';
$view_icon = '<span class="tinyicon t_viewicon"></span>';
$edit_state_icon = '<span class="tinyicon t_changestateicon"></span>';

foreach($revisions as $rev) {
  $edit_state_link = '';
  $compare_live = '';
  $compare = '';

  $view_link = l($view_icon,"node/{$rev->nid}/revisions/{$rev->vid}/view",array('html' => true, 'attributes' => array( 'title' => t('View revision @rev.',array('@rev' => $rev->vid) ))));
  $revision_link = l($rev->vid,"node/{$rev->nid}/revisions/{$rev->vid}/view",array('html' => true, 'attributes' => array( 'title' => t('View revision @rev.',array('@rev' => $rev->vid) ))));

  if(_content_moderation_statechange_allowed($rev->vid) !== false) {
    $edit_state_link = l($edit_state_icon,_content_moderation_change_state_link($rev->vid,$rev->nid),array('html' => true, 'attributes' => array( 'title' => t('Change state of revision @rev.',array('@rev' => $rev->vid) ))));
  }
  if(module_exists('diff')){
    $live = _content_moderation_live_revision($node->nid);
    if($live != NULL) {
      $compare_live = l($rev_icon,"node/{$rev->nid}/revisions/view/{$live->vid}/{$rev->vid}",array('html' => true, 'attributes' => array( 'title' => t('Compare this revision with the current live revision.') ) ));
    }
    $compare = l($rev_icon,$links['compare'],array('html' => true, 'attributes' => array( 'title' => t('List all revisions.') ) ));
  }
  $compare = 'compare';

  if($rev->state != NULL) {
    $state = ucfirst(t($rev->state));
  }
  else {
    $state  = ucfirst(t('none'));
  }
?>
<span class="revision"><?php print $revision_link?>: (<?php print $state?>) <?php print $view_link?><?php print $edit_state_link?><?php print $compare_live?></span>
<?php
}
?>
</div>