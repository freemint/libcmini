/*
 * ldiv.c
 *
 *  Created on: 12.07.2017
 *      Author: og
 */

#include <stdlib.h>


ldiv_t
ldiv(long n, long d)
{
	ldiv_t r;

	r.quot = n / d;
	r.rem  = n % d;

	return r;
}
