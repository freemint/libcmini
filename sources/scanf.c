#include <stdarg.h>
#include <stdlib.h>
#include <osbind.h>
#include <stdio.h>


extern int doscan(int (*)(void*), int (*)(int, void*), void *stream, const char *fmt, va_list va);

static int getchar_func(void* dummy);
static int ungetchar_func(int c, void* dummy);


int
scanf(const char *fmt, ...)
{
    int ret;

    va_list va;
    va_start(va, fmt);
    ret = doscan(getchar_func, ungetchar_func, NULL, fmt, va);
    va_end(va);

    return ret;
}


int
getchar_func(void* dummy)
{
    return getchar();
}


int
ungetchar_func(int c, void* dummy)
{
    return ungetc(c, stdin);
}
