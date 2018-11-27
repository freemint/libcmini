/* from Dale Schumacher's dLibs library */

/* if you chage the behaviour so that it does not reverse in-place
 * please change mktemp.c too (it assumes rev is in-place)
 */

#include <string.h>

char *
strrev (char *string)
{
	register char *p = string;
	register char *q;

	if (*(q = p)) /* non-empty string? */
	{
		while (*++q)
			;

		while (--q > p)
		{
			register char c;

			c = *q;
			*q = *p;
			*p++ = c;
		}
	}

	return string;
}
