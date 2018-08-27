<?php

function arg($n)
{
    static $args = null;
    if($args === null){
        $args = explode('/', get_uri());
    }
    return isset($args[$n])?$args[$n]:null;
}

function get_uri()
{
    static $u=null;
    if($u === null)
    {
        
        global $self,$base, $args;
        
        $uri = trim($_SERVER['REQUEST_URI'], '/');
        //decode the url
        $uri = urldecode($uri);
        $start = 0;
        if(($pos = strpos($uri, $self)) === 0)
        {
            $start = strlen($self);
        }elseif(($pos = strpos($uri, $base))===0)
        {
            $start = strlen($base);
        }
        
        $uri = trim(substr($uri, $start), '/');
        
        $u = $uri;
    }
    return $u;
}


function front_page()
{
    return "This is front page";
}

function set_menu()
{
    global $uri, $menu;
    $menus = get_all_menus();
    $menu_links = array_keys($menus);
    $menu = get_menu($uri, $menu_links);
}

function get_menu($uri, $menus)
{

    if(trim($uri) == '')
    {
        return '<front>';
    }
    $copy = explode('/', trim($uri, '/'));
    while(($t = count($copy)) > 0)
    {
        foreach($menus as $menu)
        {
            $menu = trim($menu, '/');
            $menu_args = explode('/', $menu);
            
            //if length of current uri is larger than menu link ingore it
            if($t > count($menu_args))
            {
                continue;
            }
            $match = true;
            foreach($copy as $k=>$arg)
            {
                if($menu_args[$k] == '%')
                {
                    continue;
                }
                
                if($arg != $menu_args[$k])
                {
                    $match = false;
                    break;
                }
            }
            if($match)
            {
                return $menu;
            }
        }
        array_pop($copy);
    }
    
    return null;
}


function get_all_menus(){
    static $menus = [];
    if(empty($menus)){
        
        $default_menu_args = array(
            "title" => '',
            "page callback" => '',
            "page arguments" => []
        );
        
        
        
        //call hook_menu and get menus
        foreach(module_implements('menu') as $module)
        {
            $new_menus = module_invoke($module, 'menu');
            if($new_menus && is_array($new_menus))
            {
                //add default variables
                foreach($new_menus as &$menu)
                {
                    $menu += $default_menu_args;
                }
                $menus = array_merge($menus, $new_menus);
            }
        }
    }
    return $menus;
}
