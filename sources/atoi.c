/*
 * atoi.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>
#include <ctype.h>

int atoi(const char *c)
{
    int value = 0;
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
        value += (int) (*c - '0');
        c++;
    }

    if (negative)
    {
        value=-value;
    }

    return value;
}
