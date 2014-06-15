<?php
/**
* @file
* Render a Vocabulary Index term link.
*
* Available variables:
* $term          Regular term object with the following extra properties:
* - $path        The path to the term's page. Note: this is no alias!
* - $node_count  The amount of nodes in the term. 0 if node counts are disabled.
* $vi            The VI object for the vocabulary currently being viewed.
* $dir           The text direction. Either 'rtl' or 'ltr'.
*/

// We only render a link if the term contains nodes.
if ($term->node_count != 0) {
  $term->name .= $vi->node_count ? ' <span dir="' . $dir . '">(' . $term->node_count . ')</span>' : NULL;
  // Tree views show term descriptions as link titles (tooltips), while other
  // views show them directly under the term names.
  if ($vi->view != VOCABINDEX_VIEW_TREE && $term->description) {
    $term->name .= '<span class="description">' . $term->description . '</span>';
  }
  else {
    $title = $term->description;
  }

  echo l($term->name, $term->path, array('html' => TRUE, 'attributes' => array('title' => $title)));
}
?>
