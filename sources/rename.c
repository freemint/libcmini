/*
 * rename.c
 *
 *  Created on: 12.07.2017
 *      Author: og
 */

#include <stdio.h>
#include <osbind.h>

int
rename(const char* oldname, const char* newname)
{
	return Frename(0, oldname, newname);
}
