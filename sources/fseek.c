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


extern FILE* __stdio_head;


int
fseek(FILE* fp, long offset, int origin)
{
    long res;

    if (fp && fp->__magic == _IOMAGIC) {
        res = Fseek(offset, FILE_GET_HANDLE(fp), origin);

        if (res >= 0) {
            res = 0;

            fp->__eof         = 0;
            fp->__pushed_back = 0;
            fp->__pushback    = '\0';
        }
    } else {
        res = -1;
    }

    return res;    /* FIXME: add stdio error codes */
}

