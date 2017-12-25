#include <stdlib.h>


static unsigned long _seed = 1;

int rand(void)
{
	return (int)lrand();
}

long lrand(void)
{
	_seed = (_seed >> 1) ^ (-(_seed & 1u) & 0x80200003u);
	return (_seed - 1) & 0x7fffffff;
}

void srand48(unsigned int seed)
{
	if (seed == 0) seed = -1;
	_seed = seed;
}

