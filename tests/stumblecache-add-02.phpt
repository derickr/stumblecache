--TEST--
Adding an item with too much data.
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

var_dump( $cache->add( 50, "This text field has quite a little bit too much data. Oh no!" . str_repeat( 'xxxxx', 50 ) ) );
var_dump( $cache->fetch( 50 ) );

unlink( $path );
?>
--EXPECT--
bool(false)
NULL
