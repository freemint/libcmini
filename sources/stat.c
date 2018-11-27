#include <stdlib.h>
#include <osbind.h>

#ifdef __MINTLIB_COMPATIBLE
#	include <sys/stat.h>
#else
#	include <ext.h>
#endif

int stat(const char *path, struct stat *buff)
{
    _DTA dta;
    long ret;
    const int FA_ATTRIB = (FA_DIR | FA_RDONLY | FA_HIDDEN | FA_SYSTEM);

    Fsetdta(&dta);

    ret = Fsfirst(path, FA_ATTRIB);
    if (ret == 0)
    {
        buff->st_dev = 0;
        buff->st_ino = 0;
        buff->st_mode = dta.dta_attribute;
        buff->st_nlink = 0;
        buff->st_uid = 0;
        buff->st_gid = 0;
        buff->st_rdev = 0;
        buff->st_size = dta.dta_size;
        buff->st_atime = dta.dta_time;
        buff->st_mtime = dta.dta_time;
        buff->st_ctime = dta.dta_time;
    }
    return ret;
}

