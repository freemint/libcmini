#include <stdio.h>
#include <errno.h>
#include <mint/osbind.h>
#include "lib.h"

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t bytes;
	long rc;

	if (size == 0 || nmemb == 0)
		return 0;

	bytes = size * nmemb;
	rc = Fread(FILE_GET_HANDLE(stream), bytes, ptr);

	if (rc < 0)
	{
		stream->__error = 1;
		__set_errno(-rc);
		rc = 0;
	} else
	{
		if (rc < bytes)
			stream->__eof = 1;
		rc /= size;
	}
	return rc;
}
