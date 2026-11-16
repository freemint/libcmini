#include <stdio.h>
#include <errno.h>
#include <mint/osbind.h>
#include "lib.h"

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t bytes;
	size_t got = 0;
	long rc;

	if (size == 0 || nmemb == 0)
		return 0;

	bytes = size * nmemb;

	if (stream->__pushback != EOF)
	{
		/* a character pushed back by ungetc() comes first */
		*(unsigned char *)ptr = (unsigned char)stream->__pushback;
		stream->__pushback = EOF;
		ptr = (unsigned char *)ptr + 1;
		--bytes;
		++got;
	}

	rc = bytes > 0 ? Fread(FILE_GET_HANDLE(stream), bytes, ptr) : 0;

	if (rc < 0)
	{
		stream->__error = 1;
		__set_errno(-rc);
	} else
	{
		if ((size_t)rc < bytes)
			stream->__eof = 1;
		got += rc;
	}
	return got / size;
}
