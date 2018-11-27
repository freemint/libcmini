#include <stdio.h>
#include <stdarg.h>
#include <osbind.h>

/*
 * it should be possible to call Fopen() on filenames like "PRN:", "CON:" and "AUX", but
 * it isn't neccessary since these file descriptors are always open (as -3, -1 and -2, respectively)
 */
static int _open_v(const char *filename, int access, va_list argp)
{
    long res;

    res = Fopen(filename, access & 3);  /* mask out open modes Fopen() doesn't understand */

    if (res < 0)    /* we have an error */
    {
        /* TODO: set errno accordingly */

        return -1;
    }
    return res & 0xffff;        /* return GEMDOS file handle */
}

int open(const char *filename, int access, ...)
{
    va_list args;
    int retval;

    va_start(args, access);
    retval = _open_v(filename, access, args);

    return retval;
}
