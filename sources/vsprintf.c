#include <stdio.h>
#include <values.h>
#include <stdarg.h>

extern int doprnt(int (*)(int, void *), void *stream, const char *fmt, va_list va);
extern int addchar(int c, void *stream);

struct _mem_stream { char *xstring; char *xestring; };

int vsprintf(char *str, const char *format, va_list va)
{
	struct _mem_stream stream;

	stream.xstring = str;
	stream.xestring = 0;

	doprnt(addchar, &stream, format, va);
	*stream.xstring++ = '\0';

	return stream.xestring - stream.xstring;
}
