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

$user = user_load($live->revision_uid);

$edit_icon = '<span class="tinyicon t_editicon"></span>';
$rev_icon = '<span class="tinyicon t_revisionicon"></span>';
$edit_state_icon = '<span class="tinyicon t_changestateicon"></span>';
$delete_icon = '<span class="tinyicon t_deleteicon"></span>';
$view_icon = '<span class="tinyicon t_viewicon"></span>';

/* actions */
if(_content_moderation_statechange_allowed($node->vid)) {
  $edit_state = l($edit_state_icon,$links['edit_state'],array('html' => true, 'attributes' => array( 'title' => t('Edit the state of this revision (review, approve).') ) ));
}
$edit_live = l($edit_icon,$links['edit_live'],array('html' => true, 'attributes' => array( 'title' => t('Edit the moderation status of this revision.') ) ));

$view_live_link = l($view_icon,$links['live_view'],array('html' => true, 'attributes' => array( 'title' => t('View live revision') ) ));
$view_current_link = l($view_icon,"node/{$live->nid}/revisions/{$node->vid}/view",array('html' => true, 'attributes' => array( 'title' => t('View this revision.') ) ));

if(module_exists('diff')){
  $compare_live = l($rev_icon,$links['compare_with_live'],array('html' => true, 'attributes' => array( 'title' => t('Compare this revision with the live revision.') ) ));
  $compare = l($rev_icon,$links['compare'],array('html' => true, 'attributes' => array( 'title' => t('List all revisions.') ) ));
}
// TODO: first see how revision deleting, esp. the live ones, should be handled
//$delete_current = l($delete_icon,$links['delete_revision'],array('html' => true, 'attributes' => array( 'title' => t('Delete this revision') ) ));

?>
<div id="content_moderation">
  <h4><?php print t('Live')?></h4>
   <div class="info live_info"><?php print $live->vid?>: <?=$live_link.$view_live_link.$edit_live.$compare?><br/>
    <span class="details">&raquo; <?php print date('d.m.y',$live->changed)?> (<?php print $user->name?>)</span>
  </div>
  <?php if($node->vid != $live->vid) {?>
  <h4><?php print t('Viewing')?></h4>
  <div class="info"><label><?php print $node->vid?>:</label> (<?php print $state?>) <?php print $view_current_link.$edit_state.$compare_live.$delete_current?><br/>
    <span class="details">&raquo; <?php print date('d.m.y',$live->revision_timestamp)?> (<?php print $user->name?>)</span>
    <span class="details state">&raquo; Status: <?php print t($state)?></span>
  </div>
  <?php }?>

  <?php if($revisions_list != "") { ?>
  <h4><?php print t('Pending')?></h4>
  nk you! Your patch will be included in the next release<?php
      echo $revisions_list;
   } ?>
</div>
