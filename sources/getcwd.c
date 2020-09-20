/*
 * getcwd.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ext.h>
#include <errno.h>
#include <string.h>
#include <osbind.h>
#include "lib.h"


char* getcwd(char* buffer, int bufflen)
{
    char* buf = NULL;
    char path[MAXPATH];

    path[0] = 'A' + Dgetdrv();
    path[1] = ':';

    if (Dgetpath(&path[2], 0) < 0)
    {
        __set_errno(ENODEV);
    } else if (bufflen <= strlen(path))
    {
        __set_errno(ERANGE);
    } else
    {
        if (buffer != NULL)
            buf = buffer;
        else
            buf = malloc(bufflen);

        if (buf != NULL)
        {
            strncpy(buf, path, bufflen);
            buf[bufflen] = '\0';
        }
    }

    return buf;
}
