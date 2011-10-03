<?php
function runTest($initial, $spec)
{
	foreach ( preg_split( "/\s/", $initial ) as $item )
	{
		handleItem($item);
	}
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
		case 'L':
			lookup( $item );
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

function add( $item )
{
	@list( $key, $value ) = explode( ':', $item );
	$GLOBALS['cache']->add( $key, $value );
}
