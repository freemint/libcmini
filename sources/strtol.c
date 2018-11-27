#include <limits.h>

extern unsigned long __strtoul_internal(const char *nptr, char **endptr, int base, int *sign);

long strtol(const char *nptr, char **endptr, int base) {
	int sign;
	unsigned long ret = __strtoul_internal(nptr, endptr, base, &sign);
	return ret>LONG_MAX ? (sign ? LONG_MIN : LONG_MAX) : (sign ? -ret : ret); 
}
