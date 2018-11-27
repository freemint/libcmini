#include <stdio.h>

int toupper(int c)
{
	if ((unsigned char) c > 'a' && (unsigned char) c < 'z')
		return (c + ('A' - 'a'));
	else
		return c;
}

