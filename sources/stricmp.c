#include <string.h>

#define toupper(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - ('a' - 'A')) : (c))

int stricmp(const char *s1, const char *s2)
{
	int c1, c2, d=0;
	for ( ; ; ++s1, ++s2)
	{
		c1 = toupper(*s1);
		c2 = toupper(*s2);
		if (((d = c1 - c2) != 0) || (c2 == '\0'))
			break;
	}
	return d;
}
int strcasecmp(const char *s1, const char *s2) __attribute__((alias("stricmp")));
