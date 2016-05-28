<?php
/**
 * @file
 * Plugin to allow blocks to be disabled.
 */
class BlockDisabler extends LayoutRendererEditor {
  /**
   * Render the links to display when editing a block.
   */
  protected function getBlockLinks($block) {
    $links = array();
    $links['configure'] = array(
      'title' => t('Configure'),
      'href' => $this->getUrl('configure-block', $block->uuid),
      'attributes' => array(
        'class' => array('use-ajax'),
        'data-dialog' => TRUE,
        'data-dialog-options' => json_encode(array('dialogClass' => 'layout-dialog')),
      ),
    );

    $links['remove'] = array(
      'title' => t('Remove'),
      'href' => $this->getUrl('remove-block', $block->uuid),
      'query' => array('token' => backdrop_get_token('layout-' . $this->layout->name)),
      'attributes' => array(
        'class' => array('remove-block', 'use-ajax'),
      ),
    );

    $links['hide'] = array(
      'title' => empty($block->settings['hidden']) ? t('Disable') : t('Enable'),
      'href' => $this->getUrl('hide-block', $block->uuid),
      'query' => array('token' => backdrop_get_token('layout-' . $this->layout->name)),
      'attributes' => array(
        'class' => array('use-ajax'),
      ),
    );
    $dropbutton = array(
      '#type' => 'dropbutton',
      '#links' => $links,
    );

    return backdrop_render($dropbutton);
  }

  /**
   * {@inheritdoc}
   */
  function renderBlock($block) {
    $buttons = $this->getBlockLinks($block);
    $content = $block->getAdminPreview();
    $hidden = !empty($block->settings['hidden']);
    $hidden_tag = $hidden ? t('(hidden)') : '';

    $class = 'layout-editor-block';
    if (isset($this->layout->title_block) && $this->layout->title_block == $block->uuid) {
      $class .= ' layout-editor-block-is-title';
    }
    if ($hidden) {
      $class .= ' layout-editor-block-hidden';
    }
    $class .= ' clearfix';

    $output = '<div class="' . $class . '" id="layout-editor-block-' . $block->uuid . '" data-block-id="' . $block->uuid . '">';
    $output .= '<div class="layout-editor-block-title clearfix">';
    $output .= '<span class="text">' . $block->getAdminTitle() . ' ' . $hidden_tag . '</span>';
    if ($buttons) {
      $output .= '<span class="buttons">' . $buttons . '</span>';
    }
    $output .= '</div>'; // layout-block-title
    $output .= '<div class="layout-editor-block-content">' . render($content) . '</div>';
    $output .= '</div>'; // layout-block

    return $output;
  }

}
