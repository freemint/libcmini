/*
 * strncpy.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>

char *strncpy(char *dst, const char *src, size_t max)
{
    char *ptr = dst;

    while (max-- > 0 && (*dst++ = *src++) != '\0');
    return ptr;
}
