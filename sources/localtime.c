#include <time.h>
#include <osbind.h>

#ifdef __MINTLIB_COMPATIBLE
# include <sys/time.h>
#endif /* __MINTLIB_COMPATIBLE */


extern time_t get_timezone_offset();


struct tm*
localtime(const time_t* timep)
{
    time_t t = *timep + get_timezone_offset();
    return gmtime(&t);
}
