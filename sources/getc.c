#include <stdio.h>

#ifndef __GNUC__
/* with GCC, getc() is an alias of fgetc() defined in fgetc.c */
int  getc(FILE *stream)
{
    return fgetc(stream);
}
#endif
