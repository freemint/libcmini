/*
 * setftime.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ext.h>
#include <errno.h>
#include <osbind.h>
#include "lib.h"


int setftime(int handle, struct ftime* ftimep)
{
	if (handle < 0 || Fseek(0, handle, SEEK_CUR) < 0)
    {
        __set_errno(EBADF);
        return -1;
	}

	Fdatime(ftimep, handle, 1);

	return 0;
}
