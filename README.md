# Mess with FFI

This project aims to show how to mess with PHP.

## How ?

It uses the [FFI](https://www.php.net/ffi) extension to access current PHP process
memory and changes it to override pointers to what does which function, or change
a class `final` state, or what so ever...

## Requirements

You will need PHP 7.4 installed with FFI extension.

You can source the `.alias` file to 

## Testing

### Internal functions override 

```bash
php7.3 console random --max 200 # Will output random value
php7.4 console random --max 200 # Will always output the max value
```

### Class definition changes : un-final a class

```bash
php7.3 console no-final # Will throw an exception
php7.4 console no-final # Will be ok
```

## It does not work on my machine

FFI needs correct C structures, defined in hashes. So this project may crash on non-unix
or non 64-bits OS, because type sizes or definitions might defer from the current one in
[`zend.h`](src/ffi/zend.h) shipped file.

## Ooops

PHP internal are complex, and should not be accessed this way. So it might fail because
of either bad implementation of this project, and may crash. 
