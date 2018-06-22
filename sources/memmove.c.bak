/*
 * memmove.c
 *
 *  Created on: 23.03.2014
 *      Author: ardi
 */

#include <stdlib.h>
#include <string.h>

void *memmove(void *dest, const void *src, size_t n)
{
	register unsigned char *pd = dest;
	register const unsigned char *ps = src;
	if(pd>ps)
		for (pd += n, ps += n; n--;)
			*--pd = *--ps;
	else if(pd<ps)
		while(n--)
			*pd++ = *ps++;
	return dest;
}


