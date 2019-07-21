#include <string.h>

void *memchr(const void *ptr, int ch, size_t count)
{
    const char *p = ptr;

    while (count-- > 0) {
        if (*p == (char) ch) {
            return (void *) p;
        }

        ++p;
    }

    return NULL;
}