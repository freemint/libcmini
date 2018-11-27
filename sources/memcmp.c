/*
 * strncmp.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
	int i;
	int cmp;

	for (i = 0; i < n; i++)
	{
		cmp = (* (unsigned char *) s1++ - * (unsigned char *) s2++);
		if (cmp != 0) return cmp;
	}
	return cmp;
}


