<?php
function runTest($initial, $spec)
{
	foreach ( preg_split( "/\s/", $initial ) as $item )
	{
		handleItem($item);
	}
	foreach ( preg_split( "/\s/", $spec ) as $item )
	{
		handleItem($item);
	}
	quit();
}

function handleItem($item)
{
	$type = $item[0];
	$item = substr( $item, 1 );
	switch ( $type )
	{
		case 'A':
			add( $item );
			break;
		case 'D':
			dump();
			break;
		case 'I':
			info();
			break;
		case 'F':
			fetch( $item );
			break;
		case 'Q':
			quit( $item );
			break;
		case 'R':
			remove( $item );
			break;
		case 'S':
			setup( $item );
			break;
		case 'U':
			update( $item );
			break;
	}
}

function setup( $item )
{
	list( $name, $order, $items, $dataSize ) = explode( ':', $item );
	$options = array(
		'order' => $order,
		'max_items' => $items,
		'max_datasize' => $dataSize,
	);
	$GLOBALS['cache'] = new StumbleCache( $name, $options );
}

function quit()
{
	$file = $GLOBALS['cache']->getPath();
	unset( $GLOBALS['cache'] );
	unlink( $file );
}

function info()
{
	print_r( $GLOBALS['cache']->getInfo() );
}

function add( $item )
{
	@list( $key, $value ) = explode( ':', $item );
	$GLOBALS['cache']->add( $key, $value );
}

function remove( $item )
{
	$GLOBALS['cache']->remove( $item );
}

function dump()
{
	$GLOBALS['cache']->dump();
}

function fetch( $item )
{
	echo $GLOBALS['cache']->fetch( $item ) ? "$item: FOUND\n" : "$item: NOT FOUND\n";
}
