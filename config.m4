dnl $Id: config.m4,v 1.2 2002-10-23 18:55:10 derick Exp $
dnl config.m4 for extension stumblecache

PHP_ARG_ENABLE(stumblecache, whether to enable stumblecache support,
[  --enable-stumblecache    Enable stumblecache support])

if test "$PHP_STUMBLECACHE" != "no"; then
  PHP_NEW_EXTENSION(stumblecache, stumblecache.c btree/btree.c, $ext_shared)
  PHP_SUBST(STUMBLECACHE_SHARED_LIBADD)
  PHP_ADD_BUILD_DIR($ext_builddir/lib, 1)
fi
