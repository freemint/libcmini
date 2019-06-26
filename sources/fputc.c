#include <stdio.h>


int
fputc(int c, FILE* stream)
{
    unsigned char uc = (unsigned char)c;

    fwrite(&uc, sizeof(char), 1, stream);

    return stream->__error ? EOF : c;
}
