#include <stdio.h>

int fflush(FILE *stream)
{
    if (stream == NULL)     /* NULL means "all applicable streams" here */
    {
    }
    return 0;
}

