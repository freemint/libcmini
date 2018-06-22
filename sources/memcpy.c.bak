/*
 * memcpy.c
 *
 *  Created on: 16.06.2013
 *      Author: mfro
 *  Rewritten on: 23.03.2014
 *      Author: ardi
 */

#include <stdlib.h>
#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	register char *dp = dest;
	register const char *sp = src;

	while (n--)
		*dp++ = *sp++;
	return dest;
}


