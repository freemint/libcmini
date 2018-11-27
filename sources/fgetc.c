/*
 * fgetc.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <stdio.h>
#include <osbind.h>
#include "lib.h"

int fgetc(FILE *stream)
{
	char ch;

	if ((Fread(FILE_GET_HANDLE(stream), 1, &ch) == 0L))
	{
		return EOF;
	}

	return (int) ch;
}
