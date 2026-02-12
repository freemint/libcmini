/*
 * strlen.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>
#undef strlen

size_t strlen(const char *s)
{
	return __inline_strlen(s);
}
