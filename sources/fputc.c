#include <stdio.h>

int fputc(int c, FILE *stream)
{
	int ret;
	unsigned char uc;

	uc = (unsigned char) c;
	ret = fwrite(&uc, 1, 1, stream);

	return (ret < 0 ? ret : uc);
}
