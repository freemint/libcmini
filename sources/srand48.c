#include <stdlib.h>


static unsigned long _seed = 1;

long lrand(void)
{
	_seed = (_seed >> 1) ^ (-(_seed & 1u) & 0x80200003u);
	return (_seed - 1) & 0x7fffffff;
}

int rand(void)
{
	return (int)lrand();
}

void srand48(long int seed)
{
	if (seed == 0) seed = -1;
	_seed = seed;
}

