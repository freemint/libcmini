#include <stdio.h>
#include <osbind.h>
#include "lib.h"


size_t
fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    long rc = Fwrite(FILE_GET_HANDLE(stream), size * nmemb, ptr);

    if (rc < 0) {
        stream->__error = 1;
        rc = 0;
    } else {
        rc /= size;
    }

    return rc;
}
