#include <stdio.h>
#include <stdarg.h>
#include "lib.h"

#ifdef STDIO_MAP_NEWLINE
static int last_char;
#endif /* defined STDIO_MAP_NEWLINE */

static int fpc(int c, void *fp)
{
	FILE* stream = fp;

#ifdef STDIO_MAP_NEWLINE
    int ret = '\0';

	if (!stream->__mode.__binary) {
		if (c == '\n' && last_char == '\r') {
            last_char = c;
            return 0;
		}

		last_char = c;
	}

	if (ret != EOF) {
		ret = putc(c, stream);
	}

	return ret;
#else
    return putc(c, stream);
#endif /* defined STDIO_MAP_NEWLINE */
}

int vfprintf(FILE *stream, const char *format, va_list ap)
{
#ifdef STDIO_MAP_NEWLINE
    last_char = '\0';
#endif /* defined STDIO_MAP_NEWLINE */

	return doprnt(fpc, stream, format, ap);
}


