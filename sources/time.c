#include <time.h>
#include <osbind.h>
#ifdef __MINTLIB_COMPATIBLE
#	include <sys/time.h>
#endif

long timezone = 0L;

#define ISDIGIT(c)  ((c) >= '0' && (c) <= '9')

time_t get_timezone_offset();

int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
    int tos_time;
    int tos_date;
    struct tm now;

    tos_time = Tgettime();
    tos_date = Tgetdate();

    now.tm_sec = (tos_time & 0x1f) * 2;
    now.tm_min = (tos_time >> 5) & 0x3f;
    now.tm_hour = (tos_time >> 11) & 0x1f;
    now.tm_mday = tos_date & 0x1f;
    now.tm_mon = ((tos_date >> 5) & 0xf) - 1;
    now.tm_year = (tos_date >> 9) + 80;
    now.tm_isdst = -1;

    if (tp != NULL)
    {
        tp->tv_sec = mktime(&now) - get_timezone_offset();
        tp->tv_usec = 0;
    }
    return 0;
}

time_t
get_timezone_offset()
{
    const char* tzval = getenv("TZ");

    if (tzval != NULL) {
        if (tzval[0] == 'U' && tzval[1] == 'T' && tzval[2] == 'C') {
            tzval += 3;

            if (*tzval == '\0') {
                // time zone is "UTC" -> no offset
                return 0;
            } else {
                int mul   = 1;
                int hours = 0;
                int mins  = 0;
                int secs  = 0;

                if (*tzval == '-') {
                    mul = -1;
                    ++tzval;
                } else if (*tzval == '+') {
                    ++tzval;
                }

                if (ISDIGIT(*tzval)) {
                    hours = *tzval++ - '0';

                    if (ISDIGIT(*tzval)) {
                        hours = 10 * hours + *tzval++ - '0';

                        if (ISDIGIT(*tzval)) {
                            // not more than two digits allowed
                            hours = 0;
                        } else if (*tzval++ == ':') {
                            // minutes given?

                            if (*tzval == '\0') {
                                // no -> invalid
                                hours = 0;
                            } else if (ISDIGIT(*tzval)) {
                                mins = *tzval++ - '0';

                                if (ISDIGIT(*tzval)) {
                                    mins = 10 * mins + *tzval++ - '0';

                                    if (ISDIGIT(*tzval)) {
                                        // not more than two digits allowed
                                        hours = 0;
                                        mins  = 0;
                                    } else if (*tzval == ':') {
                                        // seconds given?

                                        if (*tzval == '\0') {
                                            // no -> invalid
                                            hours = 0;
                                            mins  = 0;
                                        } else if (ISDIGIT(*tzval)) {
                                            secs = *tzval++ - '0';

                                            if (ISDIGIT(*tzval)) {
                                                secs = 10 * secs + *tzval++ - '0';

                                                if (ISDIGIT(*tzval)) {
                                                    // not more than two digits allowed
                                                    hours = 0;
                                                    mins  = 0;
                                                    secs  = 0;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                return (time_t)(mul * (secs + 60 * mins + 3600 * hours));
            }
        }
    }

    return (time_t)timezone;
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

