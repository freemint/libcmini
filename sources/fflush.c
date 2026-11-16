#include <stdio.h>

int fflush(FILE *stream)
{
	/*
	 * streams are unbuffered, so there is nothing to write out.
	 * fflush(NULL) means "all streams" and must not dereference the pointer.
	 */
	if (stream != NULL)
	{
		stream->__pushback = EOF;
		stream->__last_char = EOF;
	}
	return 0;
}
