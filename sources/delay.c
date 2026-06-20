/*
 * delay.c
 *
 *  Created on: 25.07.2019
 *      Author: og
 */

#include <ext.h>
#include <time.h>


/*
   Doesn't really handle milliseconds,
   since it's easier to simly use timer C (200 Hz / 5 ms).
*/

void delay(unsigned long milliseconds)
{
	clock_t cycles = milliseconds / (1000 / CLOCKS_PER_SEC);
	clock_t end    = clock() + cycles;

	do {} while (clock() < end);
    ;
}
