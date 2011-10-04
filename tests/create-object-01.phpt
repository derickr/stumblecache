--TEST--
Creating a StumbleCache object (1)
--INI--
stumblecache.location=/tmp
--FILE--
<?php
// one argument
$o = new StumbleCache('name');
echo get_class($o), "\n";
--EXPECT--
StumbleCache
