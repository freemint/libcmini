#include <time.h>
#include <osbind.h>

#ifdef __MINTLIB_COMPATIBLE
# include <sys/time.h>
#endif /* __MINTLIB_COMPATIBLE */


struct tm*
localtime(const time_t* timep)
{
    time_t t = *timep;

    tzset();
	t -= timezone;

    return gmtime(&t);
}
