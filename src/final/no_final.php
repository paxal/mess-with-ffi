<?php

const PAXAL_FFI_ZEND_ACC_FINAL = (1 <<  5);

function ffi_no_final(string $className): void
{
    $ffi = ffi();
    if (null === $ffi) {
        return;
    }

    if (!class_exists($className)) {
        @trigger_error(sprintf('Class "%s" does not exist', $className), E_USER_NOTICE);
        return;
    }

    $classToFind = ltrim(strtolower($className), '/');
    $definition = $ffi->zend_hash_str_find($ffi->executor_globals->class_table, $classToFind, strlen($classToFind));
    if (null === $definition) {
        throw new \RuntimeException(sprintf('Unable to find class "%s" declaration', $classToFind), E_USER_WARNING);
    }

    $definition->value->ce->ce_flags = $definition->value->ce->ce_flags & ~PAXAL_FFI_ZEND_ACC_FINAL;
}
