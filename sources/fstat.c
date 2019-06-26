#include <stdlib.h>
#include <osbind.h>

#ifdef __MINTLIB_COMPATIBLE
#   include <sys/stat.h>
#else
#   include <ext.h>
#endif

int fstat(int fd, struct stat *buff)
{
    long ret = 0;

    /*
        buff->st_dev = 0;
        buff->st_ino = 0;
        buff->st_mode = 0;
        buff->st_nlink = 0;
        buff->st_uid = 0;
        buff->st_gid = 0;
        buff->st_rdev = 0;
        buff->st_size = 0;
        buff->st_atime = 0;
        buff->st_mtime = ;
        buff->st_ctime = dta.dta_time;
    */
    return ret;
}
