/*
 * strncmp.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <string.h>

int strncmp(const char *s1, const char *s2, size_t max)
{
	int cmp = 0;

	while (max-- > 0 && cmp == 0) {
		cmp = (*s1 - *s2);

		if (*s1 == '\0' || *s2 == '\0') {
			break;
		} else {
			++s1;
			++s2;
		}
	}

	return cmp;
}


