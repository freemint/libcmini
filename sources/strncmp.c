/*
 * strncmp.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>

int strncmp(const char *s1, const char *s2, size_t max)
{
	int i;
	int cmp;

	for (i = 0; i < max && *s1++ && *s2++; i++)
	{
		cmp = (*s1 - *s2);
		if (cmp != 0) return cmp;
	}
	return cmp;
}


