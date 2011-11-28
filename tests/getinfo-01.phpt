--TEST--
Getting cache info (1)
--EXTENSIONS--
igbinary
--FILE--
<?php
require 'test-runner.php';

$initial = "Sgetinfo01:3:64:128";
$spec    = 'I';
runTest($initial, $spec);
?>
--EXPECT--
Array
(
    [version] => 1
    [order] => 3
    [max_items] => 32
    [item_count] => 0
    [item_size] => 256
    [node_count] => 11
)
