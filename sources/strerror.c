/*
 * strerror.c
 *
 *  Created on: 23.03.2014
 *      Author: ardi
 */

#include <string.h>
#include <stdio.h>

#define PREFIX  "Unknown error "

char*
strerror(int errnum)
{
	static char* unknown_error = PREFIX "0123456789";
	sprintf(unknown_error + strlen(PREFIX), "%d", errnum);
	return unknown_error;
}
