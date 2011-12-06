--TEST--
Adding elements so that a child overflows.
--EXTENSIONS--
igbinary
--FILE--
<?php
require 'lib/test-runner.php';

$initial = "Sadd03:3:64:128";
$spec    =
	'A65 A67 A68 A70 A72 '. // fill up the root node
	'A79 D '.               // causes split
	'A71 A74 D '.           // fill up 2nd child
	'A76 D';                // causes 2nd child to split
runTest($initial, $spec);
--EXPECT--
-------

IDX: 1
       68 
        0         2 
IDX: 0
       65        67 
IDX: 2
       70        72        79 
-------
-------

IDX: 1
       68 
        0         2 
IDX: 0
       65        67 
IDX: 2
       70        71        72        74        79 
-------
-------

IDX: 1
       68        72 
        0         2         3 
IDX: 0
       65        67 
IDX: 2
       70        71 
IDX: 3
       74        76        79 
-------
