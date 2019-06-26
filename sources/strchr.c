#include <string.h>

char *strchr(const char *str, int c)
{
    char *p = (char *) str;

    do
    {
        if (*p == c)
            return p;
    } while (*p++);

    return NULL;
}

char *index(const char *str, int c) __attribute__((alias("strchr")));
