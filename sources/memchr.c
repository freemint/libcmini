#include <string.h>

void *memchr(const void *ptr, int ch, size_t count)
{
    int i;
    unsigned char *p = (unsigned char *) ptr;

    for (i = 0; i < count; i++)
        if (*p == (unsigned char) ch)
            return (void *) p;
    
    return NULL;
}