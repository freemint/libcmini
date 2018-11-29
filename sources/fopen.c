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

FILE *__stdio_head = 0;

FILE *fopen(const char *path, const char *mode)
{
    FILE *fp;
    long fd;
    int create=0, append=0, read=0, write=0, i;

    if (mode == NULL)
        goto error;

    switch (*mode)
    {
        case 'a':
            write = create = append = 1;
            break;

        case 'w':
            write = create = 1;
            break;

        case 'r':
            read = 1;
            break;

        default:
            goto error;
    }

    for (i = 1; i < 4; ++i)
    {
        switch (*++mode)
        {
            case '\0':
                break;

            case '+':
                read = write = 1;
                continue;

            case 'b':
                /* ignore binary */;
                continue;

            case 'x':
                /* ignore exclusive*/;
                continue;
        }
        break;
    }

    if ((fp = calloc(sizeof(FILE), 1)) != NULL)
    {
        if (append)
        {
            fd = Fopen(path, write ? 1 + read : 0);
            if (fd >= 0)
            {
                Fseek(0, fd, SEEK_END);
                goto ok;
            }
        }

        if (create)
        {
            fd = Fcreate(path, 0);
            if (fd < 0)
                goto error;
            else if (read)
                goto ok;
            Fclose(fd);
        }
        if ((fd = Fopen(path, write ? 1 + read : 0)) >= 0)
            goto ok;
    }
error:
    return NULL;
ok:
    fp->__magic = _IOMAGIC;
    FILE_SET_HANDLE(fp, fd);
    fp->__next = __stdio_head;
    __stdio_head = fp;

    return fp;
}
