/*
 * fclose.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <stdio.h>
#include <stdlib.h>
#include <osbind.h>
#include "lib.h"

extern FILE *__stdio_head;

int fseek(FILE *fp, long offset, int origin)
{
	long res = -1;

	if (fp && fp->__magic == _IOMAGIC)
	{
		res = Fseek(offset, FILE_GET_HANDLE(fp), origin);
	}
	return (res >= 0 ? 0 : res);	/* FIXME: add stdio error codes */
}

