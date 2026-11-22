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
#include <ctype.h>
#include "lib.h"

/*
 * Lexical definitions.
 *
 * All lexical space is allocated dynamically.
 * The eighth/sixteenth bit of characters is used to prevent recognition,
 * and eventually stripped.
 */
#define UNLIMITED                 -1
#define PRINTF_BUFFER_SIZE        128
#define PRINTF_BUFFER_RESERVE     8     /* room for sign and prefix behind leading zeros */

/* leading zeros are only stored in buf while this is true, the rest is counted */
#define BUFFER_HAS_ROOM(p)        ((p) < buf + PRINTF_BUFFER_SIZE - PRINTF_BUFFER_RESERVE)

#define DEFAULT_DIGITS_REMAINDER  6
#define MAX_DIGITS_REMAINDER      10
#define MULTI_REMAINDER           10e9


#ifndef ONLY_INTEGER_IO
static int normalize_float(double* valptr, int round_to, int eformat);
#endif /* ONLY_INTEGER_IO */


#ifndef TRUE
#   define TRUE   1
#   define FALSE  0
#endif


#define ISDIGIT(c)  ((c) >= '0' && (c) <= '9')
#define ISUPPER(c)  ((c) >= 'A' && (c) <= 'Z')
#define ISLOWER(c)  ((c) >= 'a' && (c) <= 'z')


/*
 * Output the len characters of buf in reverse order. extra_zeros '0'
 * characters that did not fit into buf are inserted in front of the
 * zero_pos lowest characters of buf.
 */
static int put_reversed(int (*addchar)(int, void*), void* stream, int attributes,
                        const char* buf, int len, int zero_pos, int extra_zeros)
{
    int count = 0;

    while (len > 0) {
        if (len == zero_pos) {
            while (extra_zeros > 0) {
                count += addchar(attributes | '0', stream);
                --extra_zeros;
            }
        }

        count += addchar(attributes | (unsigned char)buf[--len], stream);
    }

    while (extra_zeros > 0) {
        count += addchar(attributes | '0', stream);
        --extra_zeros;
    }

    return count;
}


int doprnt(int (*addchar)(int, void*), void *stream, const char *sfmt, va_list ap)
{
    enum { INT_VAL, LONG_VAL, LONG_LONG_VAL };

    int  fmt;
    char pad           = ' ';
    int  flush_left    = FALSE;
    int  field_width   = 0;
    int  precision     = UNLIMITED;
    int  hash          = FALSE;
    char space_or_sign = '\0';
    int  do_long       = INT_VAL;
    int  do_short      = 0;      /* 1: h, 2: hh */
    int  do_long_double = FALSE;  /* L */
    int  negative      = FALSE;
    int  attributes    = 0;
    int  count         = 0;

    register const char* f;

    for (f = sfmt; *f != '\0'; ++f) {
        if (*f != '%') {
            /* then just out the char */
            count += addchar(attributes | (unsigned char)*f, stream);
        } else {
            char  buf[PRINTF_BUFFER_SIZE];
            char* bufptr;
            int   extra_zeros = 0;  /* zeros that did not fit into buf */
            int   zero_pos    = 0;  /* where to insert them, see put_reversed() */

        #ifndef ONLY_INTEGER_IO
            double floatval;
        #endif /* ONLY_INTEGER_IO */

            long          longval;
            unsigned long ulongval;

        #ifdef STDIO_WITH_LONG_LONG
            long long          longlongval;
            unsigned long long ulonglongval;
        #endif /* STDIO_WITH_LONG_LONG */

            while (1) {
                switch (*++f) {
                    case '-':
                        /* minus: flush left */
                        flush_left = TRUE;
                        continue;
                    case '+':
                        /* plus: numbers always with sign */
                        space_or_sign = '+';
                        continue;
                    case ' ':
                        /* space: numbers without sign start with space */
                        space_or_sign = ' ';
                        continue;
                    case '0':
                        /* padding with 0 rather than blank */
                        pad = '0';
                        continue;
                    case '#':
                        /* alternate form */
                        hash = TRUE;
                        continue;
                    default:
                        break;
                }

                break;
            }

            if (*f == '*') {
                /* field width */
                field_width = va_arg(ap, int);

                if (field_width < 0) {
                    flush_left  = TRUE;
                    field_width = -field_width;
                }

                ++f;
            } else {
                while (ISDIGIT(*f)) {
                    /* skip the digits */
                    field_width = field_width * 10 + *f++ - '0';
                }
            }

            if (*f == '.') {
                /* precision */
                ++f;

                if (*f == '*') {
                    precision = va_arg(ap, int);
                    ++f;

                    if (precision < 0) {
                        /* a negative precision is taken as if it were omitted */
                        precision = UNLIMITED;
                    }
                } else {
                    /* a '.' without digits means a precision of 0 */
                    precision = atoi(f);

                    while (ISDIGIT(*f)) {
                        /* skip the digits */
                        ++f;
                    }
                }
            }

            switch (*f) {
                case 'l':
                    /* long format */
                    ++do_long;
                    ++f;

                    if (*f == 'l') {
                        /* long long format */
                        ++do_long;
                        ++f;
                    }

                    break;

                case 'h':
                    /* short format, the argument is promoted to int */
                    ++do_short;
                    ++f;

                    if (*f == 'h') {
                        /* char format */
                        ++do_short;
                        ++f;
                    }

                    break;

                case 'z':
                case 't':
                    /* size_t, ptrdiff_t */
                    do_long = (sizeof(size_t) > sizeof(int)) ? LONG_VAL : INT_VAL;
                    ++f;
                    break;

                case 'j':
                    /* intmax_t */
                    do_long = LONG_LONG_VAL;
                    ++f;
                    break;

                case 'L':
                    /* long double */
                    do_long_double = TRUE;
                    ++f;
                    break;

                default:
                    break;
            }

            if (*f == '\0') {
                /* incomplete conversion at the end of the format string */
                break;
            }

            fmt = (unsigned char)*f;

            if (ISUPPER(fmt) && fmt != 'S' && fmt != 'Q' && fmt != 'X' && fmt != 'E' && fmt != 'G') {
                do_long = LONG_VAL;
                fmt    |= 0x20 /* tolower */;
            }

            bufptr = buf;

            switch (fmt) {
                /* do the format */
                case 'i':
                case 'd':
                    switch (do_long) {
                        case INT_VAL:
                            longval = (long)va_arg(ap, int);

                            if (do_short == 1) {
                                longval = (short)longval;
                            } else if (do_short > 1) {
                                longval = (signed char)longval;
                            }
                            break;

#ifdef STDIO_WITH_LONG_LONG

                        case LONG_LONG_VAL:
                            longlongval = va_arg(ap, long long);

                            if (longlongval > LONG_MAX || longlongval < LONG_MIN) {
                                /* use 64 bit arithmetic only if needed */

                                if (longlongval < 0) {
                                    negative    = TRUE;
                                    longlongval = -longlongval;
                                }

                                do {
                                    *bufptr++    = (longlongval % 10) + '0';
                                    longlongval /= 10;
                                } while (longlongval > 0);

                                longval = 0;
                            } else {
                                longval = longlongval;
                            }

                            break;

#else

                        case LONG_LONG_VAL:
                            /* no 64 bit output support, but consume the whole argument */
                            longval = (long)va_arg(ap, long long);
                            break;

#endif /* STDIO_WITH_LONG_LONG */

                        case LONG_VAL:
                        default:
                            longval = va_arg(ap, long);
                            break;

                    }

                    if (bufptr == buf) {
                        /** we have to use unsigned, because a signed value of 0x80000000
                         *  represents a decimal value of -2147483648 which cannot be
                         *  stored as a positive value of 2137483648 into a signed variable
                         */

                        unsigned long ulongval;

                        if (longval < 0) {
                            negative = TRUE;
                            ulongval = -longval;
                        } else {
                            ulongval = longval;
                        }

                        do {
                            *bufptr++ = (ulongval % 10) + '0';
                            ulongval  /= 10;
                        } while (ulongval > 0);

                        if (longval == 0 && precision == 0) {
                            /* the value 0 with precision 0 produces no digits */
                            bufptr = buf;
                        }
                    }

                    {
                        int num_zeros;
                        int num_blanks;
                        int sign_extra;

                        if (precision >= 0) {
                            num_zeros  = precision;
                            num_blanks = field_width - num_zeros;
                            sign_extra = TRUE;
                            pad        = ' ';

                            if (num_blanks < 0) {
                                num_blanks = 0;
                            }
                        } else {
                            if (pad == '0') {
                                num_zeros  = field_width;
                                num_blanks = 0;
                            } else {
                                num_zeros  = 0;
                                num_blanks = field_width;
                            }

                            sign_extra = FALSE;
                        }

                        field_width = num_zeros + num_blanks;
                        num_zeros  -= (int)(bufptr - buf);

                        if (num_zeros > 0) {
                            pad = '0';
                        }

                        if ((flush_left && precision <= 0) || num_zeros <= 0) {
                            if (negative) {
                                *bufptr++ = '-';
                                negative = FALSE;
                            } else if (space_or_sign != '\0') {
                                *bufptr++ = space_or_sign;
                            }
                        } else {
                            char* signptr;

                            while (num_zeros > 0) {
                                if (BUFFER_HAS_ROOM(bufptr)) {
                                    *bufptr++ = pad;
                                } else {
                                    ++extra_zeros;
                                }

                                if (--num_zeros == 0) {
                                    pad = ' ';
                                }
                            }

                            zero_pos = (int)(bufptr - buf);
                            signptr  = sign_extra ? bufptr : (bufptr - 1);

                            if (!sign_extra && (negative || space_or_sign != '\0')) {
                                /* the sign replaces the highest zero */
                                --zero_pos;
                            }

                            if (negative) {
                                *signptr++ = '-';
                                negative = FALSE;
                            } else if (space_or_sign != '\0') {
                                *signptr++ = space_or_sign;
                            }

                            if (sign_extra) {
                                bufptr = signptr;
                            }
                        }

                        field_width -= (int)(bufptr - buf) + extra_zeros;

                        if (!flush_left) {
                            while (field_width-- > 0) {
                                count += addchar(attributes | (unsigned char)pad, stream);
                            }
                        }

                        count += put_reversed(addchar, stream, attributes, buf, (int)(bufptr - buf), zero_pos, extra_zeros);

                        if (flush_left) {
                            while (field_width-- > 0) {
                                count += addchar(attributes | ' ', stream);
                            }
                        }
                    }

                    break;

                case 'f':
                case 'e':
                case 'E':
                case 'g':
                case 'G':

#ifdef ONLY_INTEGER_IO

                    {
                        static const char *p = ")taolf(";

                        do {
                            *bufptr++ = *p++;
                        } while (*p != '\0');

                        pad = ' ';

                        if (do_long_double) {
                            (void)va_arg(ap, long double);
                        } else {
                            (void)va_arg(ap, double);
                        }

#else

                    /* this is actually more than stupid, but does work for now */
                    if (do_long_double) {
                        floatval = (double)va_arg(ap, long double);
                    } else {
                        floatval = va_arg(ap, double); /* beware: va_arg() extends float to double! */
                    }

                    if (floatval < 0) {
                        negative = TRUE;
                        floatval = -floatval;
                    }

                    {
                        char   tmpbuf[PRINTF_BUFFER_SIZE];
                        char*  tmpbufptr;
                        int    exponent;
                        int    use_exp_format = FALSE;
                        double floatvalbak = floatval;

                        if (precision == UNLIMITED) {
                            precision = DEFAULT_DIGITS_REMAINDER;
                        } else if (precision > MAX_DIGITS_REMAINDER) {
                            precision = MAX_DIGITS_REMAINDER;
                        }

                        if (floatval != floatval || floatval - floatval != 0.0) {
                            /* NaN or infinity: normalize_float() would loop forever on infinity */
                            const char* special = (floatval != floatval) ? "nan" : "inf";

                            tmpbufptr = tmpbuf;

                            while (*special != '\0') {
                                *tmpbufptr++ = ISLOWER(fmt) ? *special : (*special & ~0x20);
                                ++special;
                            }

                            pad = ' ';
                            goto copy_float;
                        }

                        exponent = normalize_float(&floatval, precision, (fmt | 0x20 /* tolower */) == 'e');

                        switch (fmt | 0x20 /* tolower */) {
                            case 'e':
                                use_exp_format = TRUE;
                                break;

                            case 'g':
                                if (exponent < -4 || exponent >= precision) {
                                    use_exp_format = TRUE;
                                    floatval       = floatvalbak;
                                    exponent       = normalize_float(&floatval, precision, 1);
                                }

                                break;

                            default:
                                break;

                        }

                        tmpbufptr = tmpbuf;

                        if (use_exp_format) {
                            int digit = (int)(floatval * 10.0);
                            int count_nonzero;

                            if (digit == 10) {
                                digit     = 1;
                                floatval /= 10.0;
                                ++exponent;
                            }

                            *tmpbufptr = digit + '0';

                            if (*tmpbufptr++ == '0') {
                                count_nonzero = 0;
                            } else {
                                count_nonzero = 1;
                            }

                            floatval = floatval * 10.0 - (double)digit;
                            --exponent;

                            if (precision > 0 || hash) {
                                register int i;

                                *tmpbufptr++ = '.';

                                for (i = 0; i < precision; ++i) {
                                    int digit = (int)(floatval * 10.0);

                                    if (digit == 10) {
                                        digit     = 1;
                                        floatval /= 10.0;
                                        ++exponent;
                                    }

                                    *tmpbufptr = digit + '0';

                                    if (*tmpbufptr++ != '0') {
                                        ++count_nonzero;
                                    }

                                    floatval = floatval * 10.0 - (double)digit;
                                }
                            }

                            *tmpbufptr++ = ISLOWER(fmt) ? 'e' : 'E';

                            if (exponent < 0 && count_nonzero > 0) {
                                *tmpbufptr++ = '-';
                                exponent     = -exponent;
                            } else {
                                *tmpbufptr++ = '+';

                                if (exponent < 0) {
                                    exponent = 0;
                                }
                            }

                            /* at least two exponent digits, most significant first */
                            if (exponent >= 100) {
                                *tmpbufptr++ = '0' + (exponent / 100);
                            }

                            *tmpbufptr++ = '0' + ((exponent / 10) % 10);
                            *tmpbufptr++ = '0' + (exponent % 10);
                        } else {
                            int places = 0;
                            int int_places;

                            /* stop before tmpbuf overflows, the remaining integer digits are output as zeros */
                            while (exponent > 0 && places < PRINTF_BUFFER_SIZE - MAX_DIGITS_REMAINDER - PRINTF_BUFFER_RESERVE) {
                                int digit = (int)(floatval * 10.0);

                                if (digit == 10) {
                                    digit     = 1;
                                    floatval /= 10.0;
                                    ++exponent;
                                }

                                *tmpbufptr++ = digit + '0';
                                floatval     = floatval * 10.0 - (double)digit;

                                --exponent;
                                ++places;
                            }

                            if (exponent > 0) {
                                extra_zeros = exponent;
                                exponent    = 0;
                            }

                            if (places == 0) {
                                *tmpbufptr++ = '0';
                            }

                            int_places = (int)(tmpbufptr - tmpbuf);

                            if (hash || precision > 0) {
                                *tmpbufptr++ = '.';
                                places       = 0;

                                while (exponent < 0 && places < precision) {
                                    *tmpbufptr++ = '0';
                                    ++exponent;
                                    ++places;
                                }

                                while (places < precision) {
                                    int digit = (int)(floatval * 10.0);

                                    if (digit == 10) {
                                        digit     = 1;
                                        floatval /= 10.0;
                                        ++exponent;
                                    }

                                    *tmpbufptr++ = digit + '0';
                                    floatval     = floatval * 10.0 - (double)digit;
                                    ++places;
                                }
                            }

                            /* zeros that did not fit follow the integer digits */
                            zero_pos = (int)(tmpbufptr - tmpbuf) - int_places;
                        }

copy_float:
                        while (tmpbufptr > tmpbuf) {
                            *bufptr++ = *--tmpbufptr;
                        }

                        if (flush_left || pad != '0') {
                            if (negative) {
                                *bufptr++ = '-';
                            } else if (space_or_sign != '\0') {
                                *bufptr++ = space_or_sign;
                            }
                        }

#endif /* ONLY_INTEGER_IO */

                        field_width -= (int)(bufptr - buf) + extra_zeros;

                        if (!flush_left) {
                            if (pad == '0' && (negative || space_or_sign != '\0')) {
                                /* the sign goes in front of the zeros */
                                count += addchar(attributes | (unsigned char)(negative ? '-' : space_or_sign), stream);
                                --field_width;
                            }

                            while (field_width-- > 0) {
                                count += addchar(attributes | (unsigned char)pad, stream);
                            }
                        }

                        count += put_reversed(addchar, stream, attributes, buf, (int)(bufptr - buf), zero_pos, extra_zeros);

                        if (flush_left) {
                            while (field_width-- > 0) {
                                count += addchar(attributes | ' ', stream);
                            }
                        }
                    }

                    break;

                case 'p':
                    do_long = LONG_VAL;
                    hash    = TRUE;
                    fmt     = 'x';
                    /* no break */

                case 'o':
                case 'x':
                case 'X':
                case 'u':
                    {
                        const char* numbers = "0123456789abcdef";
                        const char* prefix = NULL;
                        int len_prefix = 0;
                        int no_digits  = FALSE;

                        switch (do_long) {
                            case INT_VAL:
                                ulongval = (unsigned long)va_arg(ap, unsigned int);

                                if (do_short == 1) {
                                    ulongval = (unsigned short)ulongval;
                                } else if (do_short > 1) {
                                    ulongval = (unsigned char)ulongval;
                                }
                                break;

#ifdef STDIO_WITH_LONG_LONG

                            case LONG_LONG_VAL:
                                ulonglongval = va_arg(ap, unsigned long long);

                                if(ulonglongval > ULONG_MAX) {
                                    /* use 64 bit arithmetic only if needed */
                                    unsigned long long base;

                                    switch (fmt) {
                                        case 'u':
                                        default:
                                            base = 10;
                                            break;

                                        case 'o':
                                            base = 010;
                                            break;

                                        case 'X':
                                            numbers = "0123456789ABCDEF";

                                        case 'x':
                                            base = 0x10;
                                            break;

                                    }

                                    if (ulonglongval == 0) {
                                        /* no prefix if value is 0 */
                                        hash = FALSE;
                                    }

                                    do {
                                        *bufptr++     = numbers[ulonglongval % base];
                                        ulonglongval /= base;
                                    } while (ulonglongval > 0);
                                } else {
                                    ulongval = (unsigned long)ulonglongval;
                                }

                                break;

#else

                            case LONG_LONG_VAL:
                                /* no 64 bit output support, but consume the whole argument */
                                ulongval = (unsigned long)va_arg(ap, unsigned long long);
                                break;

#endif /* STDIO_WITH_LONG_LONG */

                            case LONG_VAL:
                            default:
                                ulongval = va_arg(ap, unsigned long);
                                break;

                        }

                        if (bufptr == buf) {
                            unsigned long base;

                            switch (fmt) {
                                case 'u':
                                default:
                                    base = 10;
                                    break;

                                case 'o':
                                    base = 010;
                                    break;

                                case 'X':
                                    numbers = "0123456789ABCDEF";

                                case 'x':
                                    base = 0x10;
                                    break;

                            }

                            if (ulongval == 0) {
                                /* the value 0 with precision 0 produces no digits, except for "%#.0o" */
                                no_digits = precision == 0 && !(hash && fmt == 'o');

                                /* no prefix if value is 0 */
                                hash = FALSE;
                            }

                            do {
                                *bufptr++ = numbers[ulongval % base];
                                ulongval /= base;
                            } while (ulongval > 0);

                            if (no_digits) {
                                bufptr = buf;
                            }
                        }

                        if(hash) {
                            switch (fmt) {
                                case 'X':
                                    prefix = "X0";
                                    len_prefix = 2;
                                    break;
                                case 'x':
                                    prefix = "x0";
                                    len_prefix = 2;
                                    break;
                                case 'o':
                                    prefix = "0";
                                    len_prefix = 1;
                                    break;
                                default:
                                    break;
                            }
                        }

                        {
                            int num_zeros;
                            int num_blanks;

                            if (precision >= 0) {
                                num_zeros  = precision;
                                num_blanks = field_width - num_zeros;
                                pad        = ' ';

                                if (num_blanks < 0) {
                                    num_blanks = 0;
                                }
                            } else {
                                if (pad == '0' && !flush_left) {
                                    num_zeros  = field_width;
                                    num_blanks = 0;
                                } else {
                                    num_zeros  = 0;
                                    num_blanks = field_width;
                                }
                            }

                            field_width = num_zeros + num_blanks;
                            num_zeros  -= (int)(bufptr - buf);

                            if (prefix != NULL && (fmt == 'o' || pad == '0')) {
                                if (num_zeros > 0) {
                                    num_zeros -= len_prefix;

                                    if (num_blanks > 0) {
                                        ++num_blanks;
                                    }
                                }
                            }

                            if (num_zeros > 0) {
                                pad = '0';

                                while (num_zeros-- > 0) {
                                    if (BUFFER_HAS_ROOM(bufptr)) {
                                        *bufptr++ = pad;
                                    } else {
                                        ++extra_zeros;
                                    }

                                    if (num_zeros == 0) {
                                        pad      = ' ';
                                        zero_pos = (int)(bufptr - buf);

                                        if (prefix != NULL) {
                                            do {
                                                *bufptr++ = *prefix++;
                                            } while (*prefix != '\0');
                                        }
                                    }
                                }
                            } else if (prefix != NULL) {
                                do {
                                    *bufptr++ = *prefix++;
                                } while (*prefix != '\0');
                            }

                            field_width -= (int)(bufptr - buf) + extra_zeros;

                            if (!flush_left) {
                                while (field_width-- > 0) {
                                    count += addchar(attributes | (unsigned char)pad, stream);
                                }
                            }

                            count += put_reversed(addchar, stream, attributes, buf, (int)(bufptr - buf), zero_pos, extra_zeros);

                            if (flush_left) {
                                while (field_width-- > 0) {
                                    count += addchar(attributes | ' ', stream);
                                }
                            }
                        }
                    }

                    break;

                case 'c':
                    {
                        int ch = va_arg(ap, int);

                        /* '\0' is output as well, so it takes up one position */
                        --field_width;

                        if (!flush_left) {
                            while (field_width-- > 0) {
                                count += addchar(attributes | (unsigned char)pad, stream);
                            }
                        }

                        count += addchar(attributes | (unsigned char)ch, stream);

                        if (flush_left) {
                            while (field_width-- > 0) {
                                count += addchar(attributes | ' ', stream);
                            }
                        }
                    }

                    break;

                case 'S':
                case 's':
                    {
                        register int i;
                        int len = 0;

                        bufptr = va_arg(ap, char*);

                        if (bufptr == NULL) {
                            bufptr = "(nil)";
                        }

                        /* with a precision the string need not be NUL terminated, so don't use strlen() */
                        while ((precision == UNLIMITED || len < precision) && bufptr[len] != '\0') {
                            ++len;
                        }

                        field_width -= len;

                        if (!flush_left) {
                            while (field_width-- > 0) {
                                count += addchar(attributes | (unsigned char)pad, stream);
                            }
                        }

                        for (i = 0; i < len; ++i) {
                            count += addchar(attributes | (unsigned char)*bufptr++, stream);
                        }

                        if (flush_left) {
                            while (field_width-- > 0) {
                                count += addchar(attributes | ' ', stream);
                            }
                        }
                    }

                    break;

                case '%':
                    count += addchar(attributes | '%', stream);
                    break;

                default:
                    break;

            }

            flush_left    = FALSE;
            field_width   = 0;
            precision     = UNLIMITED;
            hash          = FALSE;
            do_long       = INT_VAL;
            do_short      = 0;
            do_long_double = FALSE;
            space_or_sign = '\0';
            negative      = FALSE;
            pad           = ' ';
        }
    }

    return count;
}


#ifndef ONLY_INTEGER_IO

int
normalize_float(double* valptr, int round_to, int eformat)
{
    double round;
    double floatval = *valptr;
    int    exponent = 0;

    for (round = 0.5; round_to > 0; --round_to) {
        round *= 0.1;
    }

    if (eformat) {
        round *= 0.1;
    }

    while (floatval >= 1.0) {
        floatval *= 0.1;

        if (!eformat) {
            round *= 0.1;
        }

        ++exponent;
    }

    if (floatval > 0.0) {
        while (floatval < 0.1) {
            floatval *= 10.0;

            if (!eformat) {
                round *= 10.0;
            }

            --exponent;
        }
    }

    floatval += round;

    if (floatval >= 1.0) {
        /* rounding carried into the next digit, e.g. 0.7 -> 1 with "%.0f" */
        floatval *= 0.1;
        ++exponent;
    }

    *valptr = floatval;

    return exponent;
}

#endif /* ONLY_INTEGER_IO */
