#include <stdlib.h>
#include <osbind.h>

struct iovec {
    void *iov_base;
    size_t iov_len;
};

size_t writev(int fd, const struct iovec *iov, int iovcnt)
{
    int i;
    size_t ret = 0;

    // TODO: error checking
    
    for (i = 0; i < iovcnt; i++)
        ret += Fwrite(fd, iov[i].iov_base, iov[i].iov_len);

    return ret;        
}