#include <stdio.h>

#ifdef __MINTLIB_COMPATIBLE
int __flshfp(FILE *stream, int c)
#else
int putc(int c, FILE *stream)
#endif
{
    return fputc(c, stream);
}
