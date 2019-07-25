/*
 * getche.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ext.h>


int getche()
{
	int ch = getch();

	if (ch == 0x1a) {
		// CTRL-Z
		ch = EOF;
	} else {
		putch(ch);
	}

	return ch;
}
