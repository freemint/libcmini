#include <string.h>
#include <stdlib.h>

char *strndup(const char *s1, size_t n)
{
	char *dup;
	size_t len;

	len = strlen(s1);

	if (len > n) len = n;
	dup = malloc(len + 1);
	
	if (dup) strncpy(dup, s1, len);

	return dup;
}

