#include <stdio.h>
#include <stdarg.h>


extern int doscan(int (*getchar_func)(void*), int (*ungetchar_func)(int, void*), void* stream, const char* fmt, va_list va);

static int getchar_func(void* fp);
static int ungetchar_func(int c, void* fp);


int
vfscanf(FILE* stream, const char* format, va_list ap)
{
    return doscan(getchar_func, ungetchar_func, stream, format, ap);
}


int
getchar_func(void* fp)
{
    return fgetc(fp);
}


int
ungetchar_func(int c, void* fp)
{
    return ungetc(c, fp);
}
