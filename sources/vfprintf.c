#include <stdio.h>
#include <stdarg.h>

int fpc(int c, void *fp)
{
	return fputc(c, (FILE *) fp);
}

extern int doprnt(int (*addchar)(int, void *), void *stream, const char *sfmt, va_list ap);

int vfprintf(FILE *stream, const char *format, va_list ap)
{
	return doprnt(fpc, stream, format, ap);
}


