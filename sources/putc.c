#include <stdio.h>

#ifdef __MINTLIB_COMPATIBLE
int __flshfp(FILE *stream, int c)
#else
int putc(int c, FILE *stream)
#endif
{
    int ret;
    unsigned char uc;

    uc = (unsigned char) c;
    ret = fwrite(&uc, 1, 1, stream);

    return (ret < 0 ? ret : uc);
}
