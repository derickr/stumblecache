--TEST--
Adding an element.
--FILE--
<?php
require 'test-runner.php';

$initial = "Sadd01:3:64:128";
$spec    = 'A65';
runTest($initial, $spec);
--EXPECT--
