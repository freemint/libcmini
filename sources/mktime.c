#include <time.h>
#ifdef __MINTLIB_COMPATIBLE
#	include <sys/time.h>
#endif

/* year range is from 1901 to 2038, so no century can occur which is not a leap year
#define is_leap(y)  ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0))
*/
#define is_leap(y)  ((y) % 4 == 0)

time_t mktime(struct tm *tm)
{
    static const int mon_days[] =
    {
        31,
        31 + 28,
        31 + 28 + 31,
        31 + 28 + 31 + 30,
        31 + 28 + 31 + 30 + 31,
        31 + 28 + 31 + 30 + 31 + 30,
        31 + 28 + 31 + 30 + 31 + 30 + 31,
        31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
        31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
        31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
        31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
        31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31
    };

	long tyears, tdays, leaps, utc_hrs;
    int leap_year;
    time_t time;

    tzset();

    leap_year = is_leap(tm->tm_year + 1900);
	tyears = tm->tm_year - 70;
	leaps = (tyears + 2) / 4;

    if (tyears < 0) {
        leaps = (tyears - 2) / 4;

        if (leap_year && tm->tm_mon > 1) {
            ++leaps;
        }
    } else {
        leaps = (tyears + 2) / 4; /* no of next two lines until year 2100. */

        if (leap_year && (tm->tm_mon == 0 || tm->tm_mon == 1)) {
            --leaps;
        }
    }

    tdays = (tm->tm_mon > 0) ? mon_days[tm->tm_mon - 1] : 0;

    tdays += tm->tm_mday - 1; /* days of month passed. */
    tdays += (tyears * 365) + leaps;

    utc_hrs = tm->tm_hour;

    time = (tdays * 86400) + (utc_hrs * 3600) + (tm->tm_min * 60) + tm->tm_sec;

#ifndef GMT_BIOS_CLOCK
	time += timezone;
#endif /* !defined GMT_BIOS_CLOCK */

	return time;
}
