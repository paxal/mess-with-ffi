<?php

function ffi(): ?FFI
{
    if (!class_exists('FFI')) {
        return null;
    }

    static $ffi = null;
    $ffi = $ffi ?? \FFI::load(__DIR__.'/zend.h');

    return $ffi;
}
