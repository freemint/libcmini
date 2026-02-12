/*
 * strcmp.c
 *
 *  Created on: 28.11.2013
 *      Author: mfro
 */

#include <string.h>
#undef strcmp

int strcmp(const char *scan1, const char *scan2)
{
	return __inline_strcmp(scan1, scan2);
}
