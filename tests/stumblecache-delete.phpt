--TEST--
Deletion with existing and non-existing items.
--EXTENSIONS--
igbinary
--FILE--
<?php
// Initialise the cache
$options = array(
	'order' => 3,
	'max_items' => 1024,
	'max_datasize' => 32,
);
$cache = new StumbleCache( 'tests-remove', $options );
$path = $cache->getPath();

var_dump( $cache->remove( 50 ) );
var_dump( $cache->add( 50, "some data" ) );
var_dump( $cache->remove( 50 ) );
var_dump( $cache->remove( 50 ) );

unlink( $path );
?>
--EXPECT--
bool(false)
bool(true)
bool(true)
bool(false)
