/*
 * strcmp.c
 *
 *  Created on: 28.11.2013
 *      Author: mfro
 */

#include <string.h>

int strcmp(const char *s1, const char *s2)
{
    int cmp;

    do {
        cmp = *s1 - *s2;
    } while (cmp == 0 && *s1++ != '\0' && *s2++ != '\0');

    return cmp;
}


