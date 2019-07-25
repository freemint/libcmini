/*
 * getch.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ext.h>
#include <stdio.h>


int getch()
{
	int ch;

	do {
		ch = fgetc(stdin);
	} while (ch == EOF);

	return ch;
}
