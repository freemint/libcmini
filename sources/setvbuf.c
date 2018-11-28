#include <stdio.h>

int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
    /* do nothing for now */
    if (stream != NULL)
    {
        if (stream->BufStart)
            fflush(stream);
        stream->BufStart = buf;
        stream->BufEnd = buf + size;    /* points to next char adress after buffer */
    }
    return 0;
}

