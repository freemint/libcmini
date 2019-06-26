/*
 * fgetc.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <stdio.h>
#include <osbind.h>
#include "lib.h"


int
fgetc(FILE* stream)
{
    if (stream != NULL && stream->__mode.__read) {
        int ret;

        if (stream->__pushed_back) {
            ret = stream->__pushback;
            stream->__pushed_back = 0;
            stream->__pushback    = '\0';
        } else {
            char ch;
            long rc = Fread(FILE_GET_HANDLE(stream), 1, &ch);

            if (rc == 0) {
                stream->__eof = 1;
                ret = EOF;
            } else {
                ret = ch;

                if (rc < 0) {
                    stream->__error = 1;
                }
            }
        }

        return ret;
    }

    return EOF;
}
