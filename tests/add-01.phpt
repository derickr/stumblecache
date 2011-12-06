--TEST--
Adding an element.
--EXTENSIONS--
igbinary
--FILE--
<?php
require 'lib/test-runner.php';

$initial = "Sadd01:3:64:128";
$spec    = 'A65 D';
runTest($initial, $spec);
--EXPECT--
-------

IDX: 0
       65 
-------
