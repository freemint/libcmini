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

void exit(int status)
{
    /* first: call all atexit-registered func's */
    for(; _num_at_exit; --_num_at_exit)
        (*_at_exit++) ();

    /* second: close all files */

    /*
     * FIXME: if FILEs ever get buffered, we need to call that.
     * Until then, it is not needed and causes unnecessary overhead
     */
#ifdef NOT_USED
    fcloseall();
#endif /* NOT_USED */

    /* third: exit*/
    (void) Pterm(status);
    while(1);	/* get rid of gcc complaining about returning from exit() */
}

