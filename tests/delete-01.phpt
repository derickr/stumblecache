--TEST--
Removing elements that cause a child collapse.
--EXTENSIONS--
igbinary
--FILE--
<?php
require 'test-runner.php';

$initial = "Sdelete01:3:64:128";
$spec    =
	'A65 A67 A68 A70 A72 '. // fill up the root node
	'A79 '.                 // causes split
	'A71 A74 '.             // fill up 2nd child
	'A76 D '.               // causes 2nd child to split
	'R71 D '.               // delete one item
	'R70 D';                // causes the collapse of a node
runTest($initial, $spec);
--EXPECT--
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
-------

IDX: 1
       68        74 
        0         2         3 
IDX: 0
       65        67 
IDX: 2
       70        72 
IDX: 3
       76        79 
-------
-------

IDX: 1
       74 
        0         3 
IDX: 0
       65        67        68        72 
IDX: 3
       76        79 
-------
