#include <mint/osbind.h>
#include <stdio.h>
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t nbytes)
{
    long result;

    result = Fread(fd, nbytes, buf);

    if (result < 0)
    {
        /* TODO: set errno accordingly */
        return -1;
    }
    return result;
}
