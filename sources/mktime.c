#include <time.h>
#ifdef __MINTLIB_COMPATIBLE
#	include <sys/time.h>
#endif

time_t mktime(struct tm *tm)
{
	const int mon_days [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	long tyears, tdays, leaps, utc_hrs;
	int i;

	tyears = tm->tm_year - 70;
	leaps = (tyears + 2) / 4;
	tdays = 0;
	for (i=0; i < tm->tm_mon; i++)
		tdays += mon_days[i];

	tdays += tm->tm_mday-1;
	tdays = tdays + (tyears * 365) + leaps;

	utc_hrs = tm->tm_hour;
	return (tdays * 86400) + (utc_hrs * 3600) + (tm->tm_min * 60) + tm->tm_sec;	
}
