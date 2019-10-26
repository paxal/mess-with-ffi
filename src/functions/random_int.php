<?php

function __random_int(int $min, int $max): int {
    return $max;
}

ffi_override('random_int', '__random_int');
