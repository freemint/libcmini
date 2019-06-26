/*
 * clearerr.c
 *
 *  Created on: 12.07.2017
 *      Author: og
 */

#include <stdio.h>


void
clearerr(FILE* stream)
{
	stream->__eof   = 0;
	stream->__error = 0;
}

