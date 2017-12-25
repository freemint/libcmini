Fastcall?
---------
Fastcall requires [patched GCC 4.6.4](https://github.com/PeyloW/gcc-4.6.4) with -mfastcall support.

GCC passes all arguments on the stack by default. The Atari ST, and even the Falcon 030, have a slow 16 bit wide memory bus.
As a result memory operations are very expensive. Most functions take only a few arguments that can be passed in registers.

-mfastcall uses d0-2/a0-1/fp0-2 for passing integer and pointer arguments when possible, and always return pointers in a0.
a2 is not used for passing arguments, but is considered clobbered, this allows a2 to be used as call target function pointer.

The result is faster code execution, and slightly smaller memory footprint. Especially when used in combination with -mshort to 
avoid library calls for most integer operations.

With -mfastcall libcmini itself is 3% smaller binary, and the tests/bench test executes 20% faster. 
