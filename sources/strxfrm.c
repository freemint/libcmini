#include <string.h>

size_t strxfrm(char *dest, const char *src, size_t n)
{
    return strncpy(dest, src, n);
}