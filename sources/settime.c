/*
 * settime.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ext.h>
#include <osbind.h>


void
setdate(struct date* dateRec)
{
    unsigned short tos_date;

    tos_date = (dateRec->da_day & 0x1f)
             | ((dateRec->da_mon & 0x1f) << 5)
             | ((dateRec->da_year - 1980) << 9)
             ;

	Tsetdate(tos_date);
}


void
settime(struct time* timeRec)
{
    unsigned short tos_time;

    tos_time = ((timeRec->ti_sec / 2) & 0x1f)
             | ((timeRec->ti_min & 0x3f) << 5)
             | ((timeRec->ti_hour & 0x1f) << 11)
             ;

	Tsettime(tos_time);
}
