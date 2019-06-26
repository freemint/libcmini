#include <osbind.h>
#include <unistd.h>


/* posix write needed by C++ in libstdc++-v3/libsupc++/pure.cc */
ssize_t
write(int __fd, __const void* __buf, size_t __n)
{
    return Fwrite(__fd, __n, __buf);
}
