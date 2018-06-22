/*
 * memset.c
 *
 *  Created on: 16.06.2013
 *      Author: mfro
 */


#include <stdlib.h>
#include <string.h>

void *memset(void *b, int c, size_t len)
{
	int i;

	for (i = 0; i < len; i++)
		((unsigned char *) b)[i] = (unsigned char) c;

	return b;
}
