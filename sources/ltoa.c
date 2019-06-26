#include <stdlib.h>
#include <string.h>

#ifndef TRUE
# define TRUE   1
# define FALSE  0
#endif

char *ltoa(long value, char *buffer, int radix)
{
	char* p   = buffer;
	int   neg = FALSE;

	if (value < 0) {
		neg = TRUE;
	}

	do {
		static const char* _numstr = "0123456789ABCDEF";
		*p++   = _numstr[value % radix];
		value /= radix;
	} while (value != 0);

	if (neg) {
		*p++ = '-';
	}

	*p = '\0';

	return strrev(buffer);
}

