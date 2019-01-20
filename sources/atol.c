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

long atol(const char *c)
{
    long value = 0;
    int negative = 0;

    while (isspace(*c)) c++;

    if (*c == '+')
    {
        c++;
    }
    else if (*c == '-')
    {
        negative = 1;
        c++;
    }

    while (isdigit(*c))
    {
        value *= 10;
        value += (long) (*c - '0');
        c++;
    }

    if (negative)
    {
        value=-value;
    }

    return value;
}
