/*
 * strcpy.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>
#undef strcpy

char *strcpy(char *dst, const char *src)
{
	return __inline_strcpy(dst, src);
}
