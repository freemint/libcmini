extern unsigned long __strtoul_internal(const char *nptr, char **endptr, int base, int *sign);

unsigned long strtoul(const char *nptr, char **endptr, int base) {
	int sign;
	unsigned long ret = __strtoul_internal(nptr, endptr, base, &sign);
	return sign ? -ret : ret;
}
