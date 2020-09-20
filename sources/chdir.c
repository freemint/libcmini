/*
 * chdir.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ext.h>
#include <errno.h>
#include <osbind.h>
#include "lib.h"


int chdir(char* filename)
{
    if (Dsetpath(filename) < 0)
    {
		__set_errno(ENOENT);
		return -1;
	}

	return 0;
}
