#include <stdio.h>
#include <stdlib.h>

size_t lseek(int fd, size_t offset, int whence)
{
    long res;

    res = Fseek(offset, fd, whence);
    return res;
}