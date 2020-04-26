#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence)
{
    long res;

    res = Fseek(offset, fd, whence);
    return res;
}
