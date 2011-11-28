--TEST--
Adding items.
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
$cache = new StumbleCache( 'tests-add', $options );
$path = $cache->getPath();

var_dump( $cache->add( 50, "some data" ) );
var_dump( $cache->add( 50, "some data" ) );

unlink( $path );
?>
--EXPECT--
bool(true)
bool(false)
