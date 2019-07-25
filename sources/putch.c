/*
 * putch.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ext.h>
#include <stdio.h>


int
putch(int c)
{
	Bconout(DEV_CONSOLE, c);
	return c;
}
