
extern unsigned long long __strtoull_internal(const char *nptr, char **endptr, int base, int *sign);

unsigned long long strtoull(const char *nptr, char **endptr, int base) {
	int sign;
	unsigned long long ret = __strtoull_internal(nptr, endptr, base, &sign);
	return sign ? -ret : ret;
}
