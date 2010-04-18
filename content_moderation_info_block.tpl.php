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


$current_user = user_load($node->revision_uid);

$edit_icon = '<span class="tinyicon t_editicon"></span>';
$rev_icon = '<span class="tinyicon t_revisionicon"></span>';
$edit_state_icon = '<span class="tinyicon t_changestateicon"></span>';
$delete_icon = '<span class="tinyicon t_deleteicon"></span>';
$view_icon = '<span class="tinyicon t_viewicon"></span>';


/* actions */
if(_content_moderation_statechange_allowed($node->vid)) {
  $edit_state = l($edit_state_icon,$links['edit_state'],array('html' => true, 'attributes' => array( 'title' => t('Edit the state of this revision.') ) ));
}

if($live != NULL) {
  $view_live_link = l($view_icon,$links['live_view'],array('html' => true, 'attributes' => array( 'title' => t('View live revision') ) ));

}

$view_current_link = l($view_icon,"node/{$node->nid}/revisions/{$node->vid}/view",array('html' => true, 'attributes' => array( 'title' => t('View this revision.') ) ));
$current_rev_link = l($node->vid,"node/{$node->nid}/revisions/{$node->vid}/view",array('html' => true, 'attributes' => array( 'title' => t('View this revision.') ) ));

if(module_exists('diff')){
  $compare_live = l($rev_icon,$links['compare_with_live'],array('html' => true, 'attributes' => array( 'title' => t('Compare this revision with the live revision.') ) ));
  $compare = l($rev_icon,$links['compare'],array('html' => true, 'attributes' => array( 'title' => t('List all revisions.') ) ));
}
// TODO: first see how revision deleting, esp. the live ones, should be handled
//$delete_current = l($delete_icon,$links['delete_revision'],array('html' => true, 'attributes' => array( 'title' => t('Delete this revision') ) ));

?>
<div id="content_moderation">

  <h4><?php print t('Live')?></h4>
  <?php if($live != NULL) {
    $live_user = user_load($live->revision_uid);
    ?>
    <div class="info live_info"><?php print $live->vid?>: <?=$live_link.$view_live_link.$compare?><br/>
    <span class="details">&raquo; <?php print format_date($live->revision_timestamp, 'small')?> (<?php print $live_user->name?>)</span>
    </div>
  <?php }  else { ?>
    <div class="info live_info"><?php print t('nothing approved yet');?></div>
  <?php } ?>


  <?php if($live == NULL || $node->vid != $live->vid) {?>
  <h4><?php print t('Viewing')?></h4>
  <div class="info"><label><?php print $current_rev_link?>:</label> (<?php print $state?>) <?php print $view_current_link.$edit_state.$compare_live.$delete_current?><br/>
    <span class="details">&raquo; <?php print format_date($node->revision_timestamp, 'small')?> (<?php print $current_user->name?>)</span>
    <span class="details state">&raquo; Status: <?php print t($state)?></span>
  </div>
  <?php }?>

  <?php if($revisions_list != "") { ?>
  <h4><?php print t('Pending')?></h4>
     <div class="info">
        <?php echo $revisions_list; ?>
     </div>
   <?php } ?>
</div>
