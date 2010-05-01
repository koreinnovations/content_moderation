<?php
// $Id$

/**
 * Form builder function for module settings.
 */
function content_moderation_admin_settings() {
	$form['content_moderation_pending_count'] = array(
    '#title' => t('Number of pending revisions to be shown in content_moderation block'),
    '#type' => 'textfield',
		'#size' => 3,
		'#maxlength' => 3,
    '#description' => t('Define how many pending revisions shall be visible in content_moderation block. This number will restrict the shown revisions cutting out oldest ones first. By setting this to 1 you\'ll only ever see the newest pending revision.'),
		'#default_value' => variable_get('content_moderation_pending_count', 5),
  );
  $form['content_moderation_moderate_latest_only'] = array(
    '#title' => t('Edit stop on revision-moderation'),
    '#type' => 'checkbox',
    '#description' => t('When you enable this, you will only be able to<br> - Change the state of the latest revision of the dokument<br> - Edit the document only if the latest state has the status none<br>Please refer to the documentation for further details<br><strong>Be aware enabling this feature if you have already moderated revisions which are not the latest!</strong><br> <strong>Only disable this feature if you exactly know what you are going to expect!</strong>'),
    '#default_value' => variable_get('content_moderation_moderate_latest_only', 1),
  );
  $form['content_moderation_force_publish'] = array(
    '#title' => t('Force publishing on new nodes'),
    '#type' => 'checkbox',
		'#description' => t('The intended default value is checked (feature by design).<br/>However, it is then impossible to first create a (unpublished) draft. By unchecking this feature content types\' publishing options may be set to "unpublished". When a draft goes "live" (transition into moderation state "live") for the first time the node will be published automatically. After that only users with the right "administer nodes" are able to unpublish it again (making it a draft).<br/>'),
		'#default_value' => variable_get('content_moderation_force_publish', 1),
  );

	return system_settings_form($form);
}
