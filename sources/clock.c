#include <time.h>
#include <mint/osbind.h>

#define _hz_200                ((unsigned long *) 0x4baL)

clock_t clock(void)
{
	long ssp;
	clock_t ret;

	ssp = Super(0L);

	ret = * (volatile clock_t *) _hz_200;

	Super(ssp);

	return ret;
}

