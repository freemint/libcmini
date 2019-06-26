/*
 * strncmp.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char* u1 = s1;
    const unsigned char* u2 = s2;

    while (n-- > 0) {
        int cmp = *u1++ - *u2++;

        if (cmp != 0) {
            return cmp;
        }
    }

    return 0;
}


