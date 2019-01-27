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

	for (; *s1++ && *s2++;)
	{
		cmp = (*s1 - *s2);
		if (cmp != 0) return cmp;
	}
	return cmp;
}


