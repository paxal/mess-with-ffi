<?php

function ffi_override(string $oldName, string $newName): void {
    static $overridden = [];
    static $registered = false;

    $ffi = ffi();
    if (null === $ffi) {
        return;
    }

    $ft = $ffi->executor_globals->function_table;

    $old = $ffi->zend_hash_str_find($ft, $oldName, strlen($oldName));
    $new = $ffi->zend_hash_str_find($ft, $newName, strlen($newName));

    // Make backup
    $ffi->zend_hash_str_del($ft, $oldName, strlen($oldName));
    $bak = $oldName.'.bak~'.((string) microtime(true));
    $ffi->zend_hash_str_add_new($ft, $bak, strlen($bak), $old);

    $ffi->zend_hash_str_add_new($ft, $oldName, strlen($oldName), $new);

    $overridden[$oldName] = $overridden[$oldName] ?? $old;

    if (!$registered) {
        $registered = true;
        register_shutdown_function(static function () use ($ffi, $ft, &$overridden): void {
            foreach ($overridden as $name => $value) {
                $ffi->zend_hash_str_del($ft, $name, strlen($name));
                $ffi->zend_hash_str_add_new($ft, $name, strlen($name), $value);
            }
        });
    }
}
