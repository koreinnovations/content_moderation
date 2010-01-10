<?
/**
 * Filter by node type
 */
class views_handler_filter_content_moderation_pending_revision extends  views_handler_filter{
  function construct() {
    parent::construct();
    $this->additional_fields['node_vid'] = array('table' => 'node', 'field' => 'vid');
    $this->additional_fields['revision_vid'] = array('table' => 'node_revision', 'field' => 'vid');
  }
  function admin_summary() { }
  function operator_form() { }
  function query() {
    //$this->ensure_my_table();
    $this->query->add_where($this->options['group'], $this->aliases['node_vid'] ." > ". $this->aliases['revision_vid'], $this->value);
  }
}

?>