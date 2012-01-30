--TEST--
Adding too many items.
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

for ( $i = 0; $i < 1025; $i++ )
{
	$res = $cache->add( $i, sprintf( "ITEM %04X\n", $i ) );
	if ( ! $res )
	{
		echo "Item $i could not be added\n";
	}
}

for ( $i = 0; $i < 1025; $i++ )
{
	$res = $cache->fetch( $i );
	if ( $res === NULL )
	{
		echo "Item $i could not be found\n";
	}
	else if ( $res != sprintf( "ITEM %04X\n", $i ) )
	{
		echo "Item $i had wrong contents: ", $res, " vs ", sprintf( "ITEM %04X\n", $i ), "\n";
	}
}

unlink( $path );
?>
--EXPECT--
Item 1024 could not be added
Item 1024 could not be found
