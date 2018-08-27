<?php

function module_list($refresh=false, $list = [])
{
    static $modules=null;
    if($modules===null || $refresh)
    {
        $modules = [];
        
        // $dir = opendir(BASE_PATH.'/modules');
        $module_files = glob(BASE_PATH.'/modules/*/*.module');
        
        foreach($module_files as $module_file)
        {
            
            $file_name = basename($module_file);
            $module_name = substr($file_name,0, strrpos($file_name, '.'));
            $modules[] = $module_name;
            include_once $module_file;
        }
    }
    
    if(!empty($list))
    {
        $modules = array_merge($modules, $list);
    }

    return $modules;
}


function module_hook_info() {
    
}

/**
* Implements module_implements().
*/
function module_implements($hook) {
    static $modules;
    if(!isset($modules[$hook]))
    {
        $modules[$hook] = [];
        foreach(module_list() as $module)
        {
            $function =  $module."_".$hook;
            if(function_exists($function))
            {
                $modules[$hook][] = $module;
            }
        }
    }
    
    return $modules[$hook];
}

/**
* Implements module_invoke().
*/
function module_invoke($module, $hook) {
    $args = func_get_args();
    //unset first two parametrs
    unset($args[0]);
    unset($args[1]);
    $function = $module."_".$hook;
    if(function_exists($function))
    {
        return call_user_func_array($function, $args);
    }
}

/**
* Implements module_invoke_all().
*/
function module_invoke_all($hook) {
    $args = func_get_args();
    //unset args[0] becuase it is $hook
    unset($args[0]);
    foreach(module_implements($hook) as $module)
    {
        $function =  $module."_".$hook;
        call_user_func_array($function, $args);
    }
}