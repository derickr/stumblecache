--TEST--
Creating a StumbleCache object (1)
--EXTENSIONS--
igbinary
--INI--
stumblecache.location=/tmp
--FILE--
<?php
// one argument
$o = new StumbleCache('name');
echo get_class($o), "\n";
$path = $o->getPath();
$o = NULL;
unlink( $path );
--EXPECT--
StumbleCache
