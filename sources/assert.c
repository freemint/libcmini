#include "stdlib.h"
#include "stdio.h"

void __assert_fail(const char * assertion, const char * file, unsigned int line, const char * function) {
    fprintf(stderr, "%s:%d: Assertation %s failed\r\n",
            file,
            line,
            (function != NULL ? function : ""));
    abort();
}


