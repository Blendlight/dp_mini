<?php 

function set_theme($name = 'moin')
{
    global $theme_path, $theme_info;
    //set the current theme
    $theme_info = get_theme_info();
    
    //load the current theme files
    $theme_path = BASE_PATH.'/themes/'.$name;
    //include the theme file
    include_once($theme_path.'/'.$name.".theme");
    
    
    $default_regions = [
        'content'=>'Content',
        'sidebar'=>'sidebar',
        'page_top'=>'Top of page',
        'footer'=>'footer section',
        'top' => 'top section for menu logo',
        'help' => 'Help section'
    ];
    
    //add current theme as module
    // module_list(false, [$name]);
    $theme_info = call_user_func($name.'_info');
    $theme_info['regions'] += $default_regions;
}

function get_theme_info($theme=null)
{
    static $theme_info = [];
    if(empty($theme_info))
    {
        $default_info = [
            'module' => '',
            'name' => '',
            'template' => null,
            'variables' => [],
            'function' => null
        ];
        //build theme info by hook_theme
        foreach(module_implements('theme') as $module)
        {
            $new_info = module_invoke($module, 'theme');
            if($new_info && is_array($new_info))
            {
                foreach($new_info as $name=>&$info)
                {
                    $info += $default_info;
                    $info['name'] = $name;
                    $info['module'] = $module;
                    if(!$info['template'])
                    {
                        $info['function'] = 'theme_'.$name;
                    }
                }
                $theme_info = array_merge_recursive($theme_info, $new_info);
            }
        }
        
    }
    
    if($theme !== null){
        return isset($theme_info[$theme])?$theme_info[$theme]:null;
    }else{
        return $theme_info;
    }
}







function start_page_output($page)
{
    global $theme_info,$theme_path;
    $type = $page['type'];
    $page_callback = $page['page callback'];
    $page_arguments = $page['page arguments'];
    $title = $page['title'];
    page_title($title);
    
    
    foreach($page_arguments as &$argument)
    {
        if(is_numeric($argument))
        {
            $argument = arg($argument);
        }
    }
    
    //build the templates
    $regions = $theme_info['regions'];
    foreach($regions as &$region)
    {
        $region = 'Region '.$region;
    }
    
    $regions['content'] = call_user_func_array($page_callback, $page_arguments);
    
    $page_tpl = theme("page", $regions);
    $html_tpl = theme("html", array('page'=>$page_tpl));
    print $html_tpl;
}

function page_title($t=null)
{
    static $title = '';
    if($t!==null)
    {
        $title = $t;
    }
    
    return $title;
}


function theme($tpl, $variables=[])
{
    global $theme_path, $theme_info;
    if($tpl === null)
    {
        echo "Invalid template null";
        exit;
    }
    $tpl_info = get_theme_info($tpl);
    if(!$tpl_info)
    {
        echo "Template not found ".$tpl;
        exit;
    }
    $template = $tpl_info['template'];
    
    if($template){
        //fisrt look in current theme
        $name = $template['name'];
        $found = false;
        if(file_exists($theme_path.'/template/'.$name))
        {
            $file = $theme_path.'/template/'.$name;
        }elseif(file_exists($theme_path.'/'.$name)){
            $file = $theme_path.'/'.$name;
        }else{
            $file = $template['path'].'/'.$name;
        }
        
        if(!file_exists($file))
        {
            echo "Template file not found";
            exit;
        }
        
        return load_template($file, $variables);
    }else{
        
        $variables += $tpl_info['variables'];
        //look for theme function in theme
        $fn = $theme_info['name']."_".$tpl_info['name'];
        if(!function_exists($fn))
        {
            $fn = $tpl_info['function'];
        }

        return call_user_func_array($fn, ['variables'=>$variables]);
        
    }
    
}

function load_template($file, $variables)
{
    ob_start();
    include $file;
    return ob_get_clean();
}

function add_css()
{
    
}

function add_js()
{
    
}