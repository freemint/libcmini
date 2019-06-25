/*
 * fcloseall.c
 *
 *  Created on: 10.07.2017
 *      Author: og
 */

#include <stdio.h>
#include <stdlib.h>
#include <osbind.h>
#include "lib.h"


extern FILE* __stdio_head;


int
fcloseall()
{
	/* close all streams */

	register FILE* f;

	for (f = __stdio_head; f != NULL; f = f->__next) {
		fclose(f);
	}

	return 0;
}
