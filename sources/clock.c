#include <time.h>
#include <mint/osbind.h>
#include <mint/sysvars.h>

clock_t clock(void)
{
	long ssp;
	clock_t ret;

	ssp = Super(0L);

	ret = *_hz_200;

	Super(ssp);

	return ret;
}

