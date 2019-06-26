#include <stdio.h>
#include <stdarg.h>

static int last_char;

static int fpc(int c, void *fp)
{
	int   ret    = '\0';
	FILE* stream = fp;

	if (!stream->__mode.__binary) {
		if (c == '\n' && last_char != '\r') {
			if (fputc('\r', stream) == EOF) {
				ret = EOF;
			}
		}

		last_char = c;
	}

	if (ret != EOF) {
		ret = fputc(c, stream);
	}

	return ret;
}

extern int doprnt(int (*addchar)(int, void *), void *stream, const char *sfmt, va_list ap);

int vfprintf(FILE *stream, const char *format, va_list ap)
{
	return doprnt(fpc, stream, format, ap);
}


