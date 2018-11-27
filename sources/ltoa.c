#include <stdlib.h>
#include <string.h>

static char _numstr[] = "0123456789ABCDEF";

char *ltoa(long value, char *buffer, int radix)
{
	char *p = buffer;

	do {
		*p++ = _numstr[value % radix];
	} while ((value /= radix) > 0);

	*p = '\0';

	return strrev(buffer);
}

