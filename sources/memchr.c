#include <string.h>

void *memchr(const void *ptr, int ch, size_t count)
{
    unsigned char *p = (unsigned char *) ptr;

    while (count-- > 0)
        if (*p == (unsigned char) ch)
            return (void *) p;

    return NULL;
}