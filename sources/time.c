#include <time.h>
#include <osbind.h>
#ifdef __MINTLIB_COMPATIBLE
#	include <sys/time.h>
#endif

int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
    int tos_time;
    int tos_date;
    struct tm now;

    tos_time = Tgettime();
    tos_date = Tgetdate();

    now.tm_sec = (tos_time & 0x1f) * 2;
    now.tm_min = (tos_time >> 5) & 0x3f;
    now.tm_hour = tos_time >> 11;
    now.tm_mday = tos_date & 0x1f;
    now.tm_mon = ((tos_date >> 5) & 0xf) - 1;
    now.tm_year = (tos_date >> 9) + 80;
    now.tm_isdst = -1;

    if (tp != NULL)
    {
        tp->tv_sec = mktime(&now);
        tp->tv_usec = 0;
    }
    return 0;
}

time_t time(time_t *tloc)
{
	struct timeval now;

	if (gettimeofday (&now, NULL) != 0)
		return ((time_t) -1);

	if (tloc)
		*tloc = now.tv_sec;

	return now.tv_sec;
}

