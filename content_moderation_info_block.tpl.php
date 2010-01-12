<?php
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
$delete_icon = '<span class="tinyicon t_deleteicon"></span>';

/* actions */
$edit_state = l($edit_icon,$links['edit_state'],array('html' => true, 'attributes' => array( 'title' => t('Edit the state of this revision like review or approval') ) ));
$edit_live = l($edit_icon,$links['edit_live'],array('html' => true, 'attributes' => array( 'title' => t('Edit the moderation status of this revision') ) ));
if(module_exists('diff')){
  $compare_live = l($rev_icon,$links['compare_with_live'],array('html' => true, 'attributes' => array( 'title' => t('Compare this revision with the live revision') ) ));
  $compare = l($rev_icon,$links['compare'],array('html' => true, 'attributes' => array( 'title' => t('List all revision') ) ));
}

//$edit_current = l($edit_icon,$links['edit_revision'],array('html' => true, 'attributes' => array( 'title' => t('Edit this revision') ) ));
$delete_current = l($delete_icon,$links['delete_revision'],array('html' => true, 'attributes' => array( 'title' => t('Delete this revision') ) ));

?>
<div id="content_moderation">
   <div class="info live_info"><?=t('Live')?>: <?=$live_link?> <?=$edit_live?> <?=$compare?><br/>
    <span class="details">&raquo; <?=date('d.m.y',$live->changed)?> (<?=$user->name?>)</span>
  </div>
  <?php if($node->vid != $live->vid) {?>
  <div class="info"><label><?=t('Revision')?>: </label> <?=l($node->vid,'node/'.$live->nid.'/'.$node->vid)?> <?=$edit_current?> <?=$compare_live?> <?=$delete_current?><br/>
    <span class="details">&raquo; <?=date('d.m.y',$live->revision_timestamp)?> (<?=$user->name?>)</span>
  </div>
  <?php }?>
  <?php if(_content_moderation_statechange_allowed($node->vid) && ($node->vid != $live->vid)) {?>
    <div class="current_state info"><label>Status: </label><?=$state?> <?=$edit_state?></div>
  <?php }?>
</div>