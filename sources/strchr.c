#include <string.h>

char *strchr(const char *str, int c)
{
    const char *p = str;

    do
    {
        if (*p == (char)c)
            return (char*)p;
    } while (*p++);

    return NULL;
}

char *index(const char *str, int c) __attribute__((alias("strchr")));
