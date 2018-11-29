#include <stdio.h>
//#include <unistd.h>
#include <osbind.h>
#include "lib.h"

/* posix write needed by C++ in libstdc++-v3/libsupc++/pure.cc */
size_t write(int __fd, __const void *__buf, size_t __n)
{
    return Fwrite(__fd, __n, __buf);
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    long ret = Fwrite(FILE_GET_HANDLE(stream), size * nmemb, ptr);
    return ret<0 ? 0 : ret;
}

