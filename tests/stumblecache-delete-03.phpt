--TEST--
Adding items after items have been removed.
--EXTENSIONS--
igbinary
--FILE--
<?php
// Initialise the cache
$options = array(
	'order' => 3,
	'max_items' => 64,
	'max_datasize' => 32,
);
$cache = new StumbleCache( 'tests-delete-02', $options );
$path = $cache->getPath();

for ( $i = 0; $i < 50; $i++ )
{
	$id = rand( 1, 100 );
	$res = $cache->add( $id, sprintf( "ITEM %04d\n", $id ) );
	echo '$cache->add( ', $id, ', "', sprintf( "ITEM %04d", $id ), "\" );\n";
}

for ( $i = 0; $i < 33; $i++ )
{
	$id = rand( 1, 100 );
	$res = $cache->remove( $id );
	echo '$cache->remove( ', $id, ', "', sprintf( "ITEM %04d", $id ), "\" );\n";
}

for ( $i = 0; $i < 50; $i++ )
{
	$id = rand( 1, 100 );
	$res = $cache->add( $id, sprintf( "ITEM %04d\n", $id ) );
	echo '$cache->add( ', $id, ', "', sprintf( "ITEM %04d", $id ), "\" );\n";
}

for ( $i = 1; $i <= 100; $i++ )
{
	$res = $cache->fetch( $i );
	if ( $res === NULL )
	{
	}
	else if ( $res != sprintf( "ITEM %04d\n", $i ) )
	{
		echo "Item $i had wrong contents: ", $res, " vs ", sprintf( "ITEM %04d\n", $i ), "\n";
	}
}

unlink( $path );
?>
--EXPECTF--
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->remove( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
$cache->add( %d, "ITEM %s" );
