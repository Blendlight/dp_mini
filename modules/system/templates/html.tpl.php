<?php 
/*
    ;avalible variables
    
*/
?><!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title><?= page_title();?></title>
    <?php  add_css(); ?>
    <?php  add_js('head'); ?>
</head>
<body>
    <?php  print $variables["page"] ?>
    <?php  add_js('footer');?>
</body>
</html>