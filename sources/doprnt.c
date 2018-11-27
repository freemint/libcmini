/*
 * doprnt.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#ifdef MINTLIB_COMPATIBLE
#	include <ctype.h>
#else
#	include "../include/ctype.h"
#endif
/*
 * Lexical definitions.
 *
 * All lexical space is allocated dynamically.
 * The eighth/sixteenth bit of characters is used to prevent recognition,
 * and eventually stripped.
 */
#define	META		0200
#define	ASCII		0177
#define	QUOTE		((char)	0200)	/* Eighth char bit used for 'ing */
#define	TRIM		0177	/* Mask to strip quote bit */
#define	UNDER		0000000	/* No extra bits to do both */
#define	BOLD		0000000	/* Bold flag */
#define	STANDOUT	META	/* Standout flag */
#define	LITERAL		0000000	/* Literal character flag */
#define	ATTRIBUTES	0200	/* The bits used for attributes */
#define	CHAR		0000177	/* Mask to mask out the character */

#define INF	32766		/* should be bigger than any field to print */

static char snil[] = "(nil)";
#ifdef ONLY_INTEGER_IO
static char sfloat[] = ")taolf(";
#endif
int doprnt(int (*addchar)(int, void *), void *stream, const char *sfmt, va_list ap)
{
    char buf[128];
    char *bp;
    const char *f;
#ifndef ONLY_INTEGER_IO
    float flt;
#endif
    long l;
    unsigned long u;
#ifdef STDIO_WITH_LONG_LONG
    long long ll;
    unsigned long long ull;
#endif
    int i;
    int fmt;
    unsigned char pad = ' ';
    const char *numbers;
    int flush_left = 0;
    int f_width = 0;
    int prec = INF;
    int hash = 0;
    int do_long = 0;
    int sign = 0;
    int attributes = 0;
    int num = 0;

    f = sfmt;
    for (; *f; f++)
    {
        if (*f != '%')
        {
            /* then just out the char */
            num += (*addchar)((int) (((unsigned char) *f) | attributes), stream);
        }
        else
        {
            f++; /* skip the % */

            if (*f == '-')
            { /* minus: flush left */
                flush_left = 1;
                f++;
            }

            if (*f == '0' || *f == '.')
            {
                /* padding with 0 rather than blank */
                pad = '0';
                f++;
            }
            if (*f == '*')
            {
                /* field width */
                f_width = va_arg(ap, int);
                f++;
            }
            else if (isdigit((unsigned char)*f))
            {
                f_width = atoi(f);
                while (isdigit((unsigned char)*f))
                    f++; /* skip the digits */
            }

            if (*f == '.')
            { /* precision */
                f++;
                if (*f == '*')
                {
                    prec = va_arg(ap, int);
                    f++;
                }
                else if (isdigit((unsigned char)*f))
                {
                    prec = atoi(f);
                    while (isdigit((unsigned char)*f))
                        f++; /* skip the digits */
                }
            }

            if (*f == '#')
            { /* alternate form */
                hash = 1;
                f++;
            }

            if (*f == 'l')
            { /* long format */
                do_long++;
                f++;
                if (*f == 'l')
                {
                    do_long++;
                    f++;
                }
            }

            fmt = (unsigned char) *f;
            if (fmt != 'S' && fmt != 'Q' && fmt != 'X' && isupper(fmt))
            {
                do_long = 1;
                fmt = tolower(fmt);
            }
            bp = buf;
            switch (fmt)
            { /* do the format */
            case 'i':
            case 'd':
                switch (do_long)
                {
                case 0:
                    l = (long) (va_arg(ap, int));
                    break;
#ifdef STDIO_WITH_LONG_LONG
                case 2:
                    ll = va_arg(ap, long long);
                    if(ll > LONG_MAX || ll < LONG_MIN) { /* use 64 bit arithmetic only if needed */
                        if (ll < 0) {
                            sign = 1;
                            ll = -ll;
                        }
                        do {
                            *bp++ = (char) (ll % 10) + '0';
                        } while ((ll /= 10) > 0);
                        l = 0;
                    } else
                        l = (long) ll;
                    break;
#endif
                case 1:
                default:
                    l = va_arg(ap, long);
                    break;
                }
                if(bp == buf) {
                    if (l < 0) {
                        sign = 1;
                        l = -l;
                    }
                    do {
                        *bp++ = (char) (l % 10) + '0';
                    } while ((l /= 10) > 0);
                }
                if (sign)
                    *bp++ = '-';
                f_width = f_width - (int) (bp - buf);
                if (!flush_left)
                    while (f_width-- > 0)
                        num += (*addchar)((int) (pad | attributes), stream);
                for (bp--; bp >= buf; bp--)
                    num += (*addchar)((int) (((unsigned char) *bp) | attributes), stream);
                if (flush_left)
                    while (f_width-- > 0)
                        (*addchar)((int) (' ' | attributes), stream);
                break;

            case 'f':
#ifdef ONLY_INTEGER_IO
                {
                    char *p = sfloat;
                    while(*p) *bp++ = *p++;
                    pad = ' ';
                    (va_arg(ap, double));
#else
                /* this is actually more than stupid, but does work for now */
                flt = (float) (va_arg(ap, double));	/* beware: va_arg() extends float to double! */
                if (flt < 0)
                {
                    sign = 1;
                    flt = -flt;
                }
                {
                    int quotient, remainder;

                    quotient = (int) flt;
                    remainder = (flt - quotient) * 10E5;

                    for (i = 0; i < 6; i++)
                    {
                        *bp++ = (char) (remainder % 10) + '0';
                        remainder /= 10;
                    }
                    *bp++ = '.';
                    do
                    {
                        *bp++ = (char) (quotient % 10) + '0';
                    } while ((quotient /= 10) > 0);
                    if (sign)
                        *bp++ = '-';
#endif
                    f_width = f_width - (int) (bp - buf);
                    if (!flush_left)
               while (f_width-- > 0)
                  num += (*addchar)((int) (pad | attributes), stream);
                for (bp--; bp >= buf; bp--)
                   num += (*addchar)((int) (((unsigned char) *bp) | attributes), stream);
                if (flush_left)
                   while (f_width-- > 0)
                      (*addchar)((int) (' ' | attributes), stream);
                }
                break;

            case 'p':
                do_long = 1;
                hash = 1;
                fmt = 'x';
                /* no break */
            case 'o':
            case 'x':
            case 'X':
            case 'u':
                numbers = "0123456789abcdef";
                switch (do_long)
                {
                case 0:
                    u = (unsigned long) (va_arg(ap, unsigned int));
                    break;
#ifdef STDIO_WITH_LONG_LONG
                case 2:
                    ull = va_arg(ap, unsigned long long);
                    if(ull > ULONG_MAX) { /* use 64 bit arithmetic only if needed */
                        switch(fmt) {
                        case 'u':
                        default:
                            do {
                                *bp++ = numbers[ull % 10];
                            } while ((ull /= 10) > 0);
                            break;
                        case 'o':
                            do {
                                *bp++ = numbers[ull % 8];
                            } while ((ull /= 8) > 0);
                            break;
                        case 'X':
                            numbers = "0123456789ABCDEF";
                        case 'x':
                            do {
                                *bp++ = numbers[ull % 16];
                            } while ((ull /= 16) > 0);
                            break;
                        }
                    } else
                        u = (unsigned long) ull;
                    break;
#endif
                case 1:
                default:
                    u = va_arg(ap, unsigned long);
                    break;
                }
                if(bp == buf) {
                    switch(fmt) {
                    case 'u':
                    default:
                        do {
                            *bp++ = numbers[u % 10];
                        } while ((u /= 10) > 0);
                        break;
                    case 'o':
                        do {
                            *bp++ = numbers[u % 8];
                        } while ((u /= 8) > 0);
                        break;
                    case 'X':
                        numbers = "0123456789ABCDEF";
                    case 'x':
                        do {
                            *bp++ = numbers[u % 16];
                        } while ((u /= 16) > 0);
                        break;
                    }
                }
                if(hash) {
                    switch(fmt) {
                    case 'X':
                    case 'x':
                        *bp++ = 'x';
                    case 'o':
                        *bp++ = '0';
                    }
                }
                i = f_width - (int) (bp - buf);
                if (!flush_left)
                    while (i-- > 0)
                        num += (*addchar)((int) (pad | attributes), stream);
                for (bp--; bp >= buf; bp--)
                    num += (*addchar)((int) (((unsigned char) *bp) | attributes), stream);
                if (flush_left)
                    while (i-- > 0)
                        num += (*addchar)((int) (' ' | attributes), stream);
                break;

            case 'c':
                i = va_arg(ap, int);
                num += (*addchar)((int) (i | attributes), stream);
                break;

            case 'S':
            case 'Q':
            case 's':
            case 'q':
                bp = va_arg(ap, char *);
                if (!bp)
                    bp = snil;
                f_width = f_width - strlen((char *) bp);
                if (!flush_left)
                    while (f_width-- > 0)
                        num += (*addchar)((int) (pad | attributes), stream);
                for (i = 0; *bp && i < prec; i++)
                {
                    if (fmt == 'q' && (*bp & QUOTE))
                        num += (*addchar)((int) ('\\' | attributes), stream);
                    num += (*addchar)(
                            (int) (((unsigned char) *bp & TRIM) | attributes), stream);
                    bp++;
                }
                if (flush_left)
                    while (f_width-- > 0)
                        num += (*addchar)((int) (' ' | attributes), stream);
                break;

            case 'a':
                attributes = va_arg(ap, int);
                break;

            case '%':
                num += (*addchar)((int) ('%' | attributes), stream);
                break;

            default:
                break;
            }
            flush_left = 0, f_width = 0, prec = INF, hash = 0, do_long = 0;
            sign = 0;
            pad = ' ';
        }
    }
    return num;
}
