/*
 * chdir.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ctype.h>
#include <ext.h>
#include <errno.h>
#include <osbind.h>
#include "lib.h"


int chdir(char* filename)
{
    if (filename == NULL)
    {
        __set_errno(EFAULT);
        return -1;
    }

    if (filename[0] == '\0')
    {
        /* empty path means root directory */
        filename = "\\";
    } else if (filename[1] == ':')
    {
        int drv;

        drv = toupper(filename[0]) - 'A';
        Dsetdrv(drv);

        filename += 2;
    }

    if (Dsetpath(filename) < 0)
    {
		__set_errno(ENOENT);
		return -1;
	}

	return 0;
}
