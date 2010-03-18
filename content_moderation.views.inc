<?php
// $Id$

/*
 * Implements hook_views_data().
 */
function content_moderation_views_data() {
  $data = array();

  $data['content_moderation_revision_state']['table']['group'] = t('Node Revision');
  $data['content_moderation_revision_state']['table']['join'] = array(
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
      'type' => 'INNER',
    ),
  );

  $data['content_moderation_revision_state']['state'] = array(
    'title' => t('Content Moderation state'),
    'help' => t('Content moderation state of the node revisision'),
    'filter' => array(
      'handler' => 'content_moderation_handler_filter_state',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

function content_moderation_views_handlers() {
  return array(
    'handlers' => array(
      'content_moderation_handler_filter_state' => array(
        'parent' => 'views_handler_filter',
      ),
    )
  );
}