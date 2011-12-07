--TEST--
Creating a StumbleCache object (1)
--EXTENSIONS--
igbinary
--INI--
stumblecache.location=/tmp
--FILE--
<?php
$options = array(
	'order' => 3,
	'max_items' => 1024,
	'max_datasize' => 32,
);
$o = new StumbleCache('name', $options);
echo get_class($o), "\n";
$path = $o->getPath();
$o = NULL;
unlink( $path );
--EXPECT--
StumbleCache
