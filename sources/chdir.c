/*
 * chdir.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ctype.h>
#include <errno.h>
#include <mint/osbind.h>
#include <unistd.h>
#include "lib.h"


int chdir(const char* filename)
{
    int old_drv = -1;

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
        old_drv = Dgetdrv();
        Dsetdrv(drv);

        filename += 2;
    }

    if (Dsetpath(filename) < 0)
    {
        if (old_drv >= 0)
        {
            /* a failing chdir() must not change the current drive */
            Dsetdrv(old_drv);
        }

		__set_errno(ENOENT);
		return -1;
	}

	return 0;
}
