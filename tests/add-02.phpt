--TEST--
Adding elements so that the root node overflows.
--FILE--
<?php
require 'test-runner.php';

$initial = "Sadd01:3:64:128";
$spec    =
	'A65 A67 A68 A70 A72 D '. // fill up the root node
	'A79 D';                  // causes split
runTest($initial, $spec);
--EXPECT--
-------

IDX: 0
       65        67        68        70        72 
-------
-------

IDX: 1
       68 
        0         2 
IDX: 0
       65        67 
IDX: 2
       70        72        79 
-------
