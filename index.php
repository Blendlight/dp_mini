<?php
define('BASE_PATH', getcwd());
require_once BASE_PATH.'/includes/bootstrap.inc';
bootstrap();
call_active_menu($menu);