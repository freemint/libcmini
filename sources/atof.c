/*
 * atof.c
 *
 *  Created on: 10.07.2017
 *      Author: og
 */

#include <stdlib.h>


#define ISSPACE(c)  ((c) == ' ' || (c) == '\t')
#define ISDIGIT(c)  ((c) >= '0' && (c) <= '9')

#ifndef TRUE
# define TRUE   1
# define FALSE  0
#endif


double
atof(const char* c)
{
	double value = 0.0;
	int    neg   = FALSE;

	while (ISSPACE(*c)) {
		++c;
	}

	if (*c == '-') {
		++c;
		neg = TRUE;
	}

	while (ISDIGIT(*c)) {
		value = value * 10.0 + (double)(*c++ - '0');
	}

	if (*c == '.') {
		double frac    = 0.0;
		double fracmul = 0.1;

		++c;

		while (ISDIGIT(*c)) {
			frac    += (double)(*c++ - '0') * fracmul;
			fracmul /= 10.0;
		}

		value += frac;
	}

	if (neg) {
		value = -value;
	}

	return value;
}
