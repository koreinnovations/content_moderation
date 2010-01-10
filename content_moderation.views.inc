<?

function content_moderation_views_data() {
  $data = array();
  $data['content_moderation']['table']['group'] = t('Node');
  $data['test']['table']['group'] = t('Node');
  $data['content_moderation']['new'] = array(
    'title' => t('NEW'),
    'help' => t('The node ID of the node.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
   
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['test']['moderate_new'] = array(
    'title' => t('Moderated_new'), // The item it appears as on the UI,
    'help' => t('Whether or not the node is moderated.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Moderated'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );



/*  $data['content_moderation']['table']['join'] = array( 
    'node_revisions' => array (
		'field' => 'nid',    
	    	'left_field' => 'nid', 
    		'left_table' => 'node_revisions'
    )
  );     

  $data['content_moderation']['pending_revisions'] = array(
    'title' => t('Pending revisions'),
    'help' => t('Check for pending revisions'),
   # 'real field' => 'nid',
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Pending'),
      'type' => 'yes-no',
      #'handler' => 'views_handler_filter_content_moderation_pending_revision',
    ),
  );*/
  return $data;
}

function content__moderationviews_handlers() {
  dsm('hi');
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views') . '/modules/node',
    ),
    'handlers' => array(
      'views_handler_field_node' => array(
        'parent' => 'views_handler_field',
      ),
      /*'views_handler_filter_content_moderation_pending_revision' => array(
        'parent' => 'views_handler_filter'
      )      */
    )
  );  
}

?>
