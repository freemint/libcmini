#include <stdio.h>
#include "lib.h"

int fileno(FILE *stream)
{
    if (stream != NULL)
        return FILE_GET_HANDLE(stream);
    else
    {
        // errno = EBADF;
        return -1;
    }

}