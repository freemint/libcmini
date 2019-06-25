/*
 * snprintf.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdarg.h>

extern int doprnt(int (*)(int, void *), void *stream, const char *fmt, va_list va);

struct _mem_stream { char *xstring; char *xestring; };
#define TO_MEMSP(stream) ((struct _mem_stream *) stream)

int addchar(int c, void *stream)
{
	if (TO_MEMSP(stream)->xestring == TO_MEMSP(stream)->xstring)
		*TO_MEMSP(stream)->xstring = '\0';
	else
		*TO_MEMSP(stream)->xstring++ = (char) c;

	return 1;
}

int vsnprintf(char *str, size_t size, const char *fmt, va_list va)
{
	struct _mem_stream stream;
	stream.xstring = str;
	stream.xestring = str + size - 1;
	doprnt(addchar, &stream, fmt, va);
	*stream.xstring = '\0';

	return stream.xstring - str;
}
