#include <stdio.h>

int fileno(FILE *stream)
{
    if (stream != NULL)
        return stream->Handle;
    else
    {
        // errno = EBADF;
        return -1;
    }
    
}