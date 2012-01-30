--TEST--
Adding items after items have been removed.
--EXTENSIONS--
igbinary
--FILE--
<?php
// Initialise the cache
$options = array(
	'order' => 5,
	'max_items' => 64,
	'max_datasize' => 32,
);
$cache = new StumbleCache( 'tests-delete-02', $options );
$path = $cache->getPath();

for ( $i = 0; $i < 80; $i++ )
{
	$res = $cache->add( $i, sprintf( "ITEM %04X\n", $i ) );
	if ( ! $res )
	{
		echo "Item $i could not be added\n";
	}
}

for ( $i = 25; $i < 50; $i++ )
{
	echo "Removing <$i>\n";
	$cache->remove( $i );
}

for ( $i = 80; $i < 100; $i++ )
{
	$res = $cache->add( $i, sprintf( "ITEM %04X\n", $i ) );
	if ( ! $res )
	{
		echo "Item $i could not be added\n";
	}
}

for ( $i = 0; $i < 100; $i++ )
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
Item 64 could not be added
Item 65 could not be added
Item 66 could not be added
Item 67 could not be added
Item 68 could not be added
Item 69 could not be added
Item 70 could not be added
Item 71 could not be added
Item 72 could not be added
Item 73 could not be added
Item 74 could not be added
Item 75 could not be added
Item 76 could not be added
Item 77 could not be added
Item 78 could not be added
Item 79 could not be added
Removing <25>
Removing <26>
Removing <27>
Removing <28>
Removing <29>
Removing <30>
Removing <31>
Removing <32>
Removing <33>
Removing <34>
Removing <35>
Removing <36>
Removing <37>
Removing <38>
Removing <39>
Removing <40>
Removing <41>
Removing <42>
Removing <43>
Removing <44>
Removing <45>
Removing <46>
Removing <47>
Removing <48>
Removing <49>
Item 25 could not be found
Item 26 could not be found
Item 27 could not be found
Item 28 could not be found
Item 29 could not be found
Item 30 could not be found
Item 31 could not be found
Item 32 could not be found
Item 33 could not be found
Item 34 could not be found
Item 35 could not be found
Item 36 could not be found
Item 37 could not be found
Item 38 could not be found
Item 39 could not be found
Item 40 could not be found
Item 41 could not be found
Item 42 could not be found
Item 43 could not be found
Item 44 could not be found
Item 45 could not be found
Item 46 could not be found
Item 47 could not be found
Item 48 could not be found
Item 49 could not be found
Item 64 could not be found
Item 65 could not be found
Item 66 could not be found
Item 67 could not be found
Item 68 could not be found
Item 69 could not be found
Item 70 could not be found
Item 71 could not be found
Item 72 could not be found
Item 73 could not be found
Item 74 could not be found
Item 75 could not be found
Item 76 could not be found
Item 77 could not be found
Item 78 could not be found
Item 79 could not be found
