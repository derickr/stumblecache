--TEST--
Testing options to the constructor.
--EXTENSIONS--
igbinary
--FILE--
<?php
// Initialise the cache
$options = array(
	array('order' => 3, 'max_items' => 1024, 'max_datasize' => 32),
	array('order' => 2, 'max_items' => 1024, 'max_datasize' => 32),
	array('order' => 3, 'max_items' => 12319781222, 'max_datasize' => 32),
	array('order' => 3, 'max_items' => 1024, 'max_datasize' => 9000000000),
	array('order' => 3, 'max_items' => 1024),
	array('max_items' => 1024, 'max_datasize' => 32),
	array('order' => 3, 'max_datasize' => 32),
);

foreach ( $options as $optionTest )
{
	try {
		$cache = new StumbleCache( 'tests-ctor', $optionTest );
		$path = $cache->getPath();
		var_dump( $cache->getInfo() );
		unlink( $path );
	} 
	catch ( Exception $e ) 
	{
		echo $e->getMessage(), "\n";
	}
}
?>
--EXPECT--
array(6) {
  ["version"]=>
  int(1)
  ["order"]=>
  int(3)
  ["max_items"]=>
  int(1024)
  ["item_count"]=>
  int(0)
  ["item_size"]=>
  int(32)
  ["node_count"]=>
  int(683)
}
StumbleCache::__construct(): The order should be in between 3 and 102, 2 was requested.
StumbleCache::__construct(): The max_items should setting be in between 1 and 1073741824, 3729846630 was requested.
StumbleCache::__construct(): The max_datasize setting should be in between 1 and 1048576, 410065408 was requested.
StumbleCache::__construct(): Not all three options are set (need: 'order', 'max_items' and 'max_datasize').
StumbleCache::__construct(): Not all three options are set (need: 'order', 'max_items' and 'max_datasize').
StumbleCache::__construct(): Not all three options are set (need: 'order', 'max_items' and 'max_datasize').
