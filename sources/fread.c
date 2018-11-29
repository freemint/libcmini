#include <stdio.h>
#include <osbind.h>
#include "lib.h"

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    long ret = read(FILE_GET_HANDLE(stream), ptr, size * nmemb);

    return ret < 0 ? 0 : ret / size;
}

