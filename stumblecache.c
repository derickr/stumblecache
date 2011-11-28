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

#include <stdint.h>

#include "php.h"
#include "zend.h"
#include "zend_alloc.h"
#include "php_stumblecache.h"
#include "php_globals.h"
#include "ext/standard/info.h"
#include "ext/igbinary/igbinary.h"

zend_function_entry stumblecache_functions[] = {
	{NULL, NULL, NULL}
};

static const zend_module_dep stumblecache_module_deps[] = {
  ZEND_MOD_REQUIRED("igbinary")
  {NULL, NULL, NULL}
};

zend_module_entry stumblecache_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER_EX, NULL,
	stumblecache_module_deps,
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

/* Variable declarations */
zend_class_entry *stumblecache_ce;
zend_object_handlers stumblecache_object_handlers;

/* Forward method declarations */
PHP_METHOD(StumbleCache, __construct);
PHP_METHOD(StumbleCache, getInfo);
PHP_METHOD(StumbleCache, getPath);
PHP_METHOD(StumbleCache, dump);
PHP_METHOD(StumbleCache, add);
PHP_METHOD(StumbleCache, remove);
PHP_METHOD(StumbleCache, fetch);

/* Reflection information */
ZEND_BEGIN_ARG_INFO_EX(arginfo_stumblecache_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stumblecache_getinfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stumblecache_getpath, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stumblecache_dump, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stumblecache_add, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stumblecache_remove, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stumblecache_fetch, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

/* StumbleCache methods */
zend_function_entry stumblecache_funcs[] = {
	PHP_ME(StumbleCache, __construct,     arginfo_stumblecache_construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(StumbleCache, getInfo,         arginfo_stumblecache_getinfo,   ZEND_ACC_PUBLIC)
	PHP_ME(StumbleCache, getPath,         arginfo_stumblecache_getpath,   ZEND_ACC_PUBLIC)
	PHP_ME(StumbleCache, dump,            arginfo_stumblecache_dump,      ZEND_ACC_PUBLIC)
	PHP_ME(StumbleCache, add,             arginfo_stumblecache_add,       ZEND_ACC_PUBLIC)
	PHP_ME(StumbleCache, remove,          arginfo_stumblecache_remove,    ZEND_ACC_PUBLIC)
	PHP_ME(StumbleCache, fetch,           arginfo_stumblecache_fetch,     ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

/* Class helper functions */
zval *stumblecache_instantiate(zend_class_entry *pce, zval *object TSRMLS_DC)
{
	Z_TYPE_P(object) = IS_OBJECT;
	object_init_ex(object, pce);
	Z_SET_REFCOUNT_P(object, 1);
	Z_UNSET_ISREF_P(object);
	return object;
}

static void stumblecache_object_free_storage(void *object TSRMLS_DC)
{
	php_stumblecache_obj *intern = (php_stumblecache_obj *) object;

	if (intern->cache) {
		btree_close(intern->cache);
		btree_free(intern->cache);
		intern->cache = NULL;
	}

	if (intern->path) {
		efree(intern->path);
		intern->path = NULL;
	}

	zend_object_std_dtor(&intern->std TSRMLS_CC);
	efree(object);
}

static inline zend_object_value stumblecache_object_new_ex(zend_class_entry *class_type, php_stumblecache_obj **ptr TSRMLS_DC)
{
	php_stumblecache_obj *intern;
	zend_object_value retval;
	zval *tmp;

	intern = emalloc(sizeof(php_stumblecache_obj));
	memset(intern, 0, sizeof(php_stumblecache_obj));
	if (ptr) {
		*ptr = intern;
	}

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
#if PHP_MINOR_VERSION > 3
	object_properties_init(&intern->std, class_type);
#else
	zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
#endif
	
	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) stumblecache_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = &stumblecache_object_handlers;
	
	return retval;
}

static zend_object_value stumblecache_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	return stumblecache_object_new_ex(class_type, NULL TSRMLS_CC);
}

static void scache_parse_options(zval *options, uint32_t *order, uint32_t *max_items, uint32_t *max_datasize)
{
	*order = 3;
	*max_items = 32;
	*max_datasize = 256;
}

static int stumblecache_initialize(php_stumblecache_obj *obj, char *cache_id, zval *options TSRMLS_DC)
{
	char *path;
	uint32_t order, max_items, max_datasize;

	/* Create filepath */
	spprintf(&path, 128, "%s/%s.scache", "/tmp", cache_id);

	obj->cache = btree_open(path);
	obj->path  = NULL;
	if (!obj->cache) {
		scache_parse_options(options, &order, &max_items, &max_datasize);
		obj->cache = btree_create(path, order, max_items, max_datasize);
		if (!obj->cache) {
			return 0;
		}
	}
	obj->path = path;
	return 1;
}

void stumblecache_register_class(TSRMLS_D)
{
	zend_class_entry ce_stumblecache;

	INIT_CLASS_ENTRY(ce_stumblecache, "StumbleCache", stumblecache_funcs);
	ce_stumblecache.create_object = stumblecache_object_new;
	stumblecache_ce = zend_register_internal_class_ex(&ce_stumblecache, NULL, NULL TSRMLS_CC);

	memcpy(&stumblecache_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
}

PHP_METHOD(StumbleCache, __construct)
{
	char *cache_id;
	int   cache_id_len;
	zval *options = NULL;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a", &cache_id, &cache_id_len, &options) == SUCCESS) {
		if (!stumblecache_initialize(zend_object_store_get_object(getThis() TSRMLS_CC), cache_id, options TSRMLS_CC)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not initialize cache.");
		}
	}
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
}

PHP_METHOD(StumbleCache, getPath)
{
	zval *object;
	php_stumblecache_obj *scache_obj;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, stumblecache_ce) == FAILURE) {
		return;
	}

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	scache_obj = (php_stumblecache_obj *) zend_object_store_get_object(object TSRMLS_CC);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);

	RETURN_STRING(scache_obj->path, 1);
}

/* Returns some simple statistics */
PHP_METHOD(StumbleCache, getInfo)
{
	zval *object;
	php_stumblecache_obj *scache_obj;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, stumblecache_ce) == FAILURE) {
		return;
	}

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	scache_obj = (php_stumblecache_obj *) zend_object_store_get_object(object TSRMLS_CC);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);

	array_init(return_value);
	add_assoc_long_ex(return_value, "version", sizeof("version"), scache_obj->cache->header->version);
	add_assoc_long_ex(return_value, "order", sizeof("order"), scache_obj->cache->header->order);
	add_assoc_long_ex(return_value, "max_items", sizeof("max_items"), scache_obj->cache->header->max_items);
	add_assoc_long_ex(return_value, "item_count", sizeof("item_count"), scache_obj->cache->header->item_count);
	add_assoc_long_ex(return_value, "item_size", sizeof("item_size"), scache_obj->cache->header->item_size);
	add_assoc_long_ex(return_value, "node_count", sizeof("node_count"), scache_obj->cache->header->node_count);
}

PHP_METHOD(StumbleCache, dump)
{
	zval *object;
	php_stumblecache_obj *scache_obj;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, stumblecache_ce) == FAILURE) {
		return;
	}

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	scache_obj = (php_stumblecache_obj *) zend_object_store_get_object(object TSRMLS_CC);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);

	btree_dump(scache_obj->cache);
}

PHP_METHOD(StumbleCache, add)
{
	zval *object;
	php_stumblecache_obj *scache_obj;
	long  key;
	zval *value;
	uint32_t data_idx;
	void *data;
	uint32_t *data_size;
	uint32_t  max_data_size;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Olz", &object, stumblecache_ce, &key, &value) == FAILURE) {
		return;
	}

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	scache_obj = (php_stumblecache_obj *) zend_object_store_get_object(object TSRMLS_CC);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);

	if (Z_STRLEN_P(value) <= scache_obj->cache->header->item_size) {
		if (btree_insert(scache_obj->cache, key, &data_idx)) {
			/* Add data */
			btree_get_data_ptr(scache_obj->cache, data_idx, (void**) &data, (uint32_t**) &data_size, &max_data_size);
			memcpy(data, Z_STRVAL_P(value), Z_STRLEN_P(value));
			*data_size = Z_STRLEN_P(value);
			RETURN_TRUE;
		}
	}
	RETURN_FALSE;
}

PHP_METHOD(StumbleCache, remove)
{
	zval *object;
	php_stumblecache_obj *scache_obj;
	long  key;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &object, stumblecache_ce, &key) == FAILURE) {
		return;
	}

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	scache_obj = (php_stumblecache_obj *) zend_object_store_get_object(object TSRMLS_CC);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);

	if (btree_delete(scache_obj->cache, key)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}

PHP_METHOD(StumbleCache, fetch)
{
	zval *object;
	php_stumblecache_obj *scache_obj;
	long  key;
	uint32_t data_idx;
	void    *data;
	uint32_t data_size;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &object, stumblecache_ce, &key) == FAILURE) {
		return;
	}

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	scache_obj = (php_stumblecache_obj *) zend_object_store_get_object(object TSRMLS_CC);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);

	if (btree_search(scache_obj->cache, scache_obj->cache->root, key, &data_idx)) {
		/* Retrieve data */
		data = btree_get_data(scache_obj->cache, data_idx, &data_size);
		RETURN_STRINGL(data, data_size, 1);
	} else {
		return; /* Implicit return NULL; */
	}
}

PHP_MINIT_FUNCTION(stumblecache)
{
	ZEND_INIT_MODULE_GLOBALS(stumblecache, stumblecache_init_globals, NULL);

	stumblecache_register_class(TSRMLS_C);

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
