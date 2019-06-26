/*
 * vsnprintf.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdarg.h>


extern int doprnt(int (*addchar)(int, void*), void* stream, const char* fmt, va_list va);

int addchar(int c, void* stream);


struct _mem_stream
{
    char* xstring;
    char* xestring;
};


static int char_count;


int
vsnprintf(char* str, size_t size, const char* fmt, va_list va)
{
    struct _mem_stream stream;
    char temp;

    if (size == 0) {
        str  = &temp;
        size = 1;
    }

    char_count = 0;

    stream.xstring  = str;
    stream.xestring = str + size - 1;

    doprnt(addchar, &stream, fmt, va);

    *stream.xstring = '\0';

    return char_count;
}


int
addchar(int c, void* stream)
{
    struct _mem_stream* memstream = (struct _mem_stream*)stream;

    *memstream->xstring = c;

    if (memstream->xstring != memstream->xestring) {
        ++memstream->xstring;
    }

    ++char_count;

    return 1;
}
