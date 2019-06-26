#include <stdio.h>
#include <values.h>
#include <stdarg.h>


struct _mem_stream
{
    const char* xstring;
    const char* xestring;
};


extern int doscan(int (*getchar_func)(void*), int (*ungetchar_func)(int, void*), void* stream, const char* fmt, va_list va);

static int getchar_func(void *fp);


int
vsscanf(const char* str, const char* format, va_list va)
{
    struct _mem_stream stream;

    stream.xstring  = str;
    stream.xestring = NULL;

    return doscan(getchar_func, NULL, &stream, format, va);
}


int
getchar_func(void* fp)
{
    struct _mem_stream* stream = fp;
    int ch = *stream->xstring;

    if (ch == '\0') {
        ch = EOF;
    } else {
        ++stream->xstring;
    }

    return ch;
}
