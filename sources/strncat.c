/*
 * strncat.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>

char *strncat(char *dst, const char *src, size_t max)
{
	int i;
	char *ret = dst;

	dst = &dst[strlen(dst)];
	for (i = 0; i < max && *src; i++)
	{
		*dst++ = *src++;
	}
	*dst++ = '\0';

	return ret;
}

