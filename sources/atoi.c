/*
 * atoi.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>
#ifdef MINTLIB_COMPATIBLE
#	include <ctype.h>
#else
#	include "../include/ctype.h"
#endif

int atoi(const char *c)
{
    int value = 0;
    while (isdigit(*c))
    {
        value *= 10;
        value += (int) (*c - '0');
        c++;
    }
    return value;
}
