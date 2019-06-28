/*
 * div.c
 *
 *  Created on: 12.07.2017
 *      Author: og
 */

#include <stdlib.h>


div_t
div(int n, int d)
{
	div_t r;

	r.quot = n / d;
	r.rem  = n % d;

	return r;
}
