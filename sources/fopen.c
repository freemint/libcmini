/*
 * fopen.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <stdio.h>
#include <stdlib.h>
#include <osbind.h>
#include "lib.h"


FILE* __stdio_head = NULL;


FILE*
fopen(const char* path, const char* mode)
{
    FILE* fp;
    long  fd;

    if (mode == NULL || path == NULL) {
        return NULL;
    }

    fp = calloc(sizeof(FILE), 1);

    if (fp == NULL) {
        return NULL;
    } else {
        register int i;

        switch (*mode) {
            case 'a':
                fp->__mode.__write  = 1;
                fp->__mode.__create = 1;
                fp->__mode.__append = 1;
                break;

            case 'w':
                fp->__mode.__write  = 1;
                fp->__mode.__create = 1;
                break;

            case 'r':
                fp->__mode.__read = 1;
                break;

            default:
                goto error;

        }

        for (i = 1; i < 4; ++i) {
            switch (*++mode) {
                case '\0':
                default:
                    break;

                case '+':
                    fp->__mode.__read  = 1;
                    fp->__mode.__write = 1;
                    continue;

                case 'b':
                    fp->__mode.__binary = 1;
                    continue;

                case 'x':
                    fp->__mode.__exclusive = 1;
                    continue;

            }

            break;
        }

        if (fp->__mode.__append) {
            fd = Fopen(path, fp->__mode.__write ? 1 + fp->__mode.__read : 0);

            if(fd >= 0) {
                Fseek(0, fd, SEEK_END);
                goto ok;
            }
        }

        if (fp->__mode.__create) {
            fd = Fcreate (path, 0);

            if (fd < 0) {
                goto error;
            } else if (fp->__mode.__read) {
                goto ok;
            }

            Fclose(fd);
        }

        fd = Fopen(path, fp->__mode.__write ? 1 + fp->__mode.__read : 0);

        if (fd >= 0) {
            goto ok;
        }
    }

error:
    free(fp);
    return NULL;

ok:
    fp->__magic = _IOMAGIC;
    FILE_SET_HANDLE(fp, fd);
    fp->__pushed_back = 0;
    fp->__pushback = '\0';
    fp->__next = __stdio_head;
    fp->__eof = 0;
    fp->__error = 0;
    __stdio_head = fp;

    return fp;
}
