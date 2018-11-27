#include <stdarg.h>
#include <stdlib.h>
#include <osbind.h>

extern int doprnt(int (*)(int, void *), void *stream, const char *fmt, va_list va);

int putchar(int c)
{
	(void) Cconout(c);
	return 1;
}

int printf(const char *fmt, ...)
{
	int ret;

	va_list va;
	va_start(va, fmt);
	ret = doprnt((int (*)(int, void *)) putchar, NULL, fmt, va);
	va_end(va);

	return ret;
}
