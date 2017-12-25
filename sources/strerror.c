/*
 * strerror.c
 *
 *  Created on: 23.03.2014
 *      Author: ardi
 */

#include <string.h>
#include <stdio.h>

static char unknown_error[] = "Unknown error 0123456789";

char *strerror(int errnum) {

	snprintf (unknown_error, sizeof(unknown_error),"Unknown error %u", errnum);
	return unknown_error;
}