#include <stdio.h>

/*
 * this was originally implemented as a static inline, but C++ expects this
 * to be callable as a function in the standard library
 */
int getc(FILE *stream)
{
    return fgetc(stream);
}
