<?php
function bootstrap(){
    //we are not going to use the cache tool
    $real_stages = [
        'set_globals',
        'load_database',
        'load_modules',
        'load_variables',
        'load_theme',
        'call_hook_bootstrap'
    ];
    
    $stages = ['load_files', 'set_globals', 'load_modules','set_menu', 'call_hook_bootstrap'];
    foreach($stages as $stage)
    {
        if(function_exists($stage))
        {
            $stage();
        }else{
            echo 'not found '.$stage;
            exit;
        }
    }
}



function set_globals()
{
    
    global $uri, $self,$base, $args, $base_uri;
    $self = trim($_SERVER['SCRIPT_NAME'], '/');
    $base1 = dirname($self);
    $base = trim($base1, '/');
    
    $uri = get_uri();
    
    $host = $_SERVER["HTTP_HOST"];
    $request_uri = $_SERVER["REQUEST_URI"];
    if($base1 == ".")
    {
        $base1 = "";
    }
    $base_uri =  "http://".$host.'/'.trim($base1, '/');
}
function load_modules()
{
    module_list();
}



function call_hook_bootstrap()
{
    module_invoke('system', 'bootstrap');
}

function load_files()
{
    include_once BASE_PATH.'/includes/common.inc';
    include_once BASE_PATH.'/includes/cache.inc';
    include_once BASE_PATH.'/includes/theme.inc';
    include_once BASE_PATH.'/includes/module.inc';
}