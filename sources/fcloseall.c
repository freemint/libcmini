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


int
fcloseall()
{
	/* close all streams */

	register FILE* f = __stdio_head;

	while (f != NULL) {
        FILE* next = f->__next;

        fclose(f);
        f = next;
    }

	return 0;
}
