/* from Dale Schumacher's dLibs library */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

/*
 * This routine is safe in the sense that it does not make
 * assumptions about sizeof(void *). Gcc assumes same as char *
 * when not -ansi, the "other" compiler just barfs.
 *
 */


void*
bsearch(const void* key, const void* base, size_t num, size_t size, int (*cmp)(const void*, const void*))
{
	register size_t a, b;

#if 0
	assert(key != NULL && base != NULL && size > 0 && num > 0 && cmp != NULL);
#else
	if (key == NULL || base == NULL || size == 0 || num == 0 || cmp == NULL) {
		return NULL;
	}
#endif

	a = 0;
	b = num - 1;

	while (a <= b) {
		register size_t c            = (a + b) / 2;
		const void*     compare_with = (char*)base + c * size;
		register int    dir          = cmp(key, compare_with);

		if (dir == 0) {
			return (void*)compare_with;
		} else if (dir < 0) {
			if (c == 0) {
				return NULL;
			}

			b = c - 1;
		} else  {
			/* (dir > 0) */
			a = c + 1;
		}
	}

	return NULL;
}
