#include <stdio.h>
#include <osbind.h>
#include "lib.h"


size_t
fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    size_t bytes = size * nmemb;
    long   rc    = Fread(FILE_GET_HANDLE(stream), bytes, ptr);

    if (rc < 0) {
        stream->__error = 1;
        rc = 0;
    } else {
        if (rc < bytes) {
            stream->__eof = 1;
        }

        rc /= size;
    }

    return rc;
}

