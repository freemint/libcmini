#ifndef _TIME_H_
#define _TIME_H_

#include <stdio.h>

#define CLOCKS_PER_SEC 200
#define CLK_TCK	       200

typedef long time_t;
typedef long clock_t;
typedef long suseconds_t;

struct tm 
{
	int tm_sec;         /* seconds */
	int tm_min;         /* minutes */
	int tm_hour;        /* hours */
	int tm_mday;        /* day of the month */
	int tm_mon;         /* month */
	int tm_year;        /* year */
	int tm_wday;        /* day of the week */
	int tm_yday;        /* day in the year */
	int tm_isdst;       /* daylight saving time */
};

struct timeval
{
	time_t tv_sec;			/* seconds */
	suseconds_t tv_usec;	/* microseconds */
};

struct timezone
{
	int tz_minuteswest;		/* minutes west of Greenwich */
	int tz_dsttime;			/* type of DST correction */
};

extern time_t time(time_t *tloc);
extern clock_t clock(void);
extern time_t mktime(struct tm *tm);

#endif /* _TIME_H_ */
