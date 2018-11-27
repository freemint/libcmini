#include <mint/osbind.h>
#include <stdio.h>


int close(int fd)
{
    long ret;

    ret = Fclose(fd);

    if (ret != 0)
    {
        /* TODO: set errno accordingly */
        return -1;
    }
    return 0;
}


