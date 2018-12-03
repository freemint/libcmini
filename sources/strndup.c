#include <string.h>
#include <stdlib.h>

char *strndup(const char *s1, size_t n)
{
	char *dup;
	size_t len;

	len = strlen(s1);

	if (len > n) len = n;
	dup = malloc(len + 1);
	
	if (dup) {
		memcpy(dup, s1, len); // use memcpy to suppress warnings
		dup[len] = '\0'; // unlike strncpy, strndup always appends '\0'
	}
	return dup;
}

