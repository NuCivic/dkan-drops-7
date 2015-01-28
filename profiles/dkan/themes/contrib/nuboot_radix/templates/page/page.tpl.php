<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see html.tpl.php
 */
?>
<header id="header" class="header" role="header">
  <div class="container">
    <?php if ($logo): ?>
      <a class="logo navbar-btn pull-left" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>
    <?php print $site_name; ?>
    <nav class="navbar navbar-default" role="navigation">
      
      <!-- Brand and toggle get grouped for better mobile display -->
      <div class="navbar-header">
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#navbar-collapse">
          <span class="sr-only"><?php print t('Toggle navigation'); ?></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
      </div> <!-- /.navbar-header -->

      <!-- Collect the nav links, forms, and other content for toggling -->
      <div class="collapse navbar-collapse" id="navbar-collapse">
        <?php if ($main_menu): ?>
          <ul id="main-menu" class="menu nav navbar-nav">
            <?php print render($main_menu); ?>
          </ul>
        <?php endif; ?>
        <!-- views exposed search -->
        <?php
          $block = block_load('dkan_sitewide', 'dkan_sitewide_search_bar');
          if($block):
            $search = _block_get_renderable_array(_block_render_blocks(array($block)));
            print render($search);
          endif;
        ?>
        <!-- dkan user menu -->
        <?php
          $block = block_load('dkan_sitewide', 'dkan_sitewide_user_menu');
          if($block):
            $search = _block_get_renderable_array(_block_render_blocks(array($block)));
            print render($search);
          endif;
        ?>
      </div><!-- /.navbar-collapse -->
    </nav><!-- /.navbar -->
  </div> <!-- /.container -->
</header>

<div id="main-wrapper frontend">
  <div id="main" class="main container">

    <header role="banner" id="page-header">
      <?php if (!empty($site_slogan)): ?>
        <p class="lead"><?php print $site_slogan; ?></p>
      <?php endif; ?>
    </header> <!-- /#page-header -->

    <?php if (!empty($breadcrumb)): print $breadcrumb; endif;?>
    <?php print $messages; ?>
    <?php if (!empty($page['help'])): ?>
      <?php print render($page['help']); ?>
    <?php endif; ?>


    <div class="main-row">

      <section> 
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if (!empty($title) && (arg(0) == 'admin' || arg(1) == 'add' || arg(1) == 'edit')): ?>
          <h1 class="page-header"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if (!empty($tabs)): ?>
          <?php print render($tabs); ?>
        <?php endif; ?>
        <?php if (!empty($action_links)): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>
        <?php print render($page['content']); ?>
      </section>

    </div>

  </div> <!-- /#main -->
</div> <!-- /#main-wrapper -->

<footer id="footer" class="footer" role="footer">
  <div class="container">
    <?php if ($copyright): ?>
      <small class="copyright pull-left"><?php print $copyright; ?></small>
    <?php endif; ?>
    <small class="pull-right"><?php print render($page['footer']); ?></small>
  </div>
</footer>