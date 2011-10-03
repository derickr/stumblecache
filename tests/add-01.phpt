--TEST--
Test for adding an element to an empty tree
--FILE--
<?php
require 'test-runner.php';

$initial = "Sadd01:3:64:128";
$spec    = 'A65';
runTest($initial, $spec);
--EXPECT--
