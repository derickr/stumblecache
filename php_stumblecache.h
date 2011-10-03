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
/* $Id: php_stumblecache.h,v 1.9 2009-03-30 18:22:15 derick Exp $ */

#ifndef PHP_STUMBLECACHE_H
#define PHP_STUMBLECACHE_H

#include "php.h"

extern zend_module_entry stumblecache_module_entry;
#define phpext_stumblecache_ptr &stumblecache_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(stumblecache);
PHP_MSHUTDOWN_FUNCTION(stumblecache);
PHP_RINIT_FUNCTION(stumblecache);
PHP_RSHUTDOWN_FUNCTION(stumblecache);
PHP_MINFO_FUNCTION(stumblecache);

ZEND_BEGIN_MODULE_GLOBALS(stumblecache)
	/* empty */
ZEND_END_MODULE_GLOBALS(stumblecache) 

typedef struct _php_stumblecache_obj php_stumblecache_obj;

struct _php_stumblecache_obj {
	zend_object   std;
	int           cache;
};

#ifdef ZTS
# define STUMBLECACHE_G(v) TSRMG(stumblecache_globals_id, zend_stumblecache_globals *, v)
#else
# define STUMBLECACHE_G(v) (stumblecache_globals.v)
#endif

#endif
