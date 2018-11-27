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

int fclose(FILE *fp)
{
	if (fp && fp->__magic == _IOMAGIC)
	{
		Fclose(FILE_GET_HANDLE(fp));
		fp->__magic = 0;

		if (__stdio_head == fp)
			__stdio_head = fp->__next;
		else {
			FILE *prev = __stdio_head;
			for (; prev && prev->__next != fp; prev = prev->__next)
				;
			if (prev) prev->__next = fp->__next;
		}
		free(fp);
	}
	return 0;
}

