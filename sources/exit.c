/*
 * exit.c
 *
 *  Created on: 31.05.2013
 *      Author: mfro
 */

#include <stdlib.h>
#include <stdio.h>
#include <osbind.h>
#include "lib.h"

ExitFn *_at_exit = NULL;
int _num_at_exit = 0;
extern FILE *__stdio_head;

void exit(int status)
{
	
	/* fist: call all atexit-registered func's */
	for(;_num_at_exit; --_num_at_exit)
		(*_at_exit++) ();
	
	/* second: close all files */
	FILE *stream = __stdio_head;
	while (stream) {  
		FILE *stream_next = __stdio_head->__next;
		if (stream->__magic == _IOMAGIC)
			fclose (stream);
		stream = stream_next;
	}
	/* third: exit*/
	(void) Pterm(status);
	while(1);	/* get rid of gcc complaining about returning from exit() */
}

