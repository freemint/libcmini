/*
 * strcmp.c
 *
 *  Created on: 28.11.2013
 *      Author: mfro
 */

#define strcmp __inline_strcmp
#include <string.h>
#undef strcmp

int	strcmp(const char *scan1, const char *scan2);

int strcmp(const char *scan1, const char *scan2)
{
#ifdef __OPTIMIZE__
	return __inline_strcmp(scan1, scan2);
#else
	unsigned char c1, c2;

	do {
		c1 = (unsigned char) *scan1++;
		c2 = (unsigned char) *scan2++;
	} while (c1 && c1 == c2);
	return c1 - c2;
#endif
}
