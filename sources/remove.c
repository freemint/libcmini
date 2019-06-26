/*
 * remove.c
 *
 *  Created on: 12.07.2017
 *      Author: og
 */


#include <errno.h>
#include <stdio.h>
#include <osbind.h>


int
remove(const char* filename)
{
	int ret = Ddelete(filename);

	if (ret == ENOTDIR) {
		ret = Fdelete(filename);
	}

	return ret;
}
