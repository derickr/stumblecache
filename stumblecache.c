/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2011 StumbleUpon Inc.                             |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Derick Rethans <derick@derickrethans.nl>                    |
   +----------------------------------------------------------------------+
 */
/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "zend.h"
#include "zend_alloc.h"
#include "php_stumblecache.h"
#include "php_globals.h"
#include "ext/standard/info.h"

zend_function_entry stumblecache_functions[] = {
	{NULL, NULL, NULL}
};


zend_module_entry stumblecache_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"stumblecache",
	stumblecache_functions,
	PHP_MINIT(stumblecache),
	PHP_MSHUTDOWN(stumblecache),
	PHP_RINIT(stumblecache),	
	PHP_RSHUTDOWN(stumblecache),
	PHP_MINFO(stumblecache),
#if ZEND_MODULE_API_NO >= 20010901
	"0.0.1",
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_STUMBLECACHE
ZEND_GET_MODULE(stumblecache)
#endif

ZEND_DECLARE_MODULE_GLOBALS(stumblecache)
/*
PHP_INI_BEGIN()
PHP_INI_END()
*/
 
static void stumblecache_init_globals(zend_stumblecache_globals *stumblecache_globals)
{
	/* Empty */
}

PHP_MINIT_FUNCTION(stumblecache)
{
	ZEND_INIT_MODULE_GLOBALS(stumblecache, stumblecache_init_globals, NULL);

	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(stumblecache)
{
	return SUCCESS;
}


PHP_RINIT_FUNCTION(stumblecache)
{
	return SUCCESS;
}


PHP_RSHUTDOWN_FUNCTION(stumblecache)
{
	return SUCCESS;
}


PHP_MINFO_FUNCTION(stumblecache)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "stumblecache support", "enabled");
	php_info_print_table_end();
}
