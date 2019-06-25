/*
 * strtod.c --
 *
 *  Source code for the "strtod" library procedure.
 *
 * Copyright (c) 1988-1993 The Regents of the University of California.
 * Copyright (c) 1994 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id: strtod.c,v 1.1.1.3 2002/04/05 16:12:51 jevans Exp $
 */

#include <stdlib.h>


#ifndef TRUE
# define TRUE   1
# define FALSE  0
#endif


#define ISSPACE(c)  ((c) == ' ' || (c) == '\t')
#define ISDIGIT(c)  ((c) >= '0' && (c) <= '9')


static int max_exponent = 511;	/* Largest possible base 10 exponent.  Any
								 * exponent larger than this will already
								 * produce underflow or overflow, so there's
								 * no need to worry about additional digits.
								 */

static double powers_of_10[] =	/* Table giving binary powers of 10.  Entry */
{								/* is 10^2^i.  Used to convert decimal      */
	1.0e1,						/* exponents into floating-point numbers.   */
	1.0e2,
	1.0e4,
	1.0e8,
	1.0e16,
	1.0e32,
	1.0e64,
	1.0e128,
	1.0e256
};


/*
 *----------------------------------------------------------------------
 *
 * strtod --
 *
 *  This procedure converts a floating-point number from an ASCII
 *  decimal representation to internal double-precision format.
 *
 * Results:
 *  The return value is the double-precision floating-point
 *  representation of the characters in string.  If endptr isn't
 *  NULL, then *endptr is filled in with the address of the
 *  next character after the last one that was part of the
 *  floating-point number.
 *
 * Side effects:
 *  None.
 *
 *----------------------------------------------------------------------
 */

double
strtod(const char* string, char** endptr)
{
	int     sign     = FALSE;
	int     exp_sign = FALSE;

	double  fraction;
	double  dblexp;
	double* d;

	register const char* p;

	int exp     = 0;        /* Exponent read from "EX" field. */
	int fracexp = 0;        /* Exponent that derives from the fractional
							 * part.  Under normal circumstatnces, it is
							 * the negative of the number of digits in F.
							 * However, if I is very long, the last digits
							 * of I get dropped (otherwise a long I with a
							 * large negative exponent could cause an
							 * unnecessary overflow on I alone).  In this
							 * case, fracexp is incremented one for each
							 * dropped digit.
							 */

	int mantsize;           /* Number of digits in mantissa. */
	int decpt;              /* Number of mantissa digits BEFORE decimal
							 * point.
							 */
	const char* pexp;       /* Temporarily holds location of exponent
							 * in string.
							 */

	/*
	 * Strip off leading blanks and check for a sign.
	 */

	p = string;

	while (ISSPACE(*p)) {
		++p;
	}

	if (*p == '-') {
		sign = TRUE;
		++p;
	} else if (*p == '+') {
		++p;
	}

	/*
	 * Count the number of digits in the mantissa (including the decimal
	 * point), and also locate the decimal point.
	 */

	decpt    = -1;
	mantsize = 0;

	for (;;) {
		char c = *p++;

		if (!ISDIGIT(c)) {
			if (c != '.' || decpt >= 0) {
				break;
			}

			decpt = mantsize;
		}

		++mantsize;
	}

	/*
	 * Now suck up the digits in the mantissa.  Use two integers to
	 * collect 9 digits each (this is faster than using floating-point).
	 * If the mantissa has more than 18 digits, ignore the extras, since
	 * they can't affect the value anyway.
	 */

	pexp  = p;
	p    -= mantsize;

	if (decpt < 0) {
		decpt = mantsize;
	} else {
		--mantsize;          /* One of the digits was the point. */
	}

	if (mantsize > 18) {
		fracexp  = decpt - 18;
		mantsize = 18;
	} else {
		fracexp  = decpt - mantsize;
	}

	if (mantsize == 0) {
		fraction = 0.0;
		p        = string;

		goto done;
	} else {
		int frac1 = 0;
		int frac2 = 0;

		while (mantsize > 9) {
			char c = *p++;

			if (c == '.') {
				c = *p++;
			}

			frac1 = 10 * frac1 + c - '0';

			--mantsize;
		}

		while (mantsize > 0) {
			char c = *p++;

			if (c == '.') {
				c = *p++;
			}

			frac2 = 10 * frac2 + c - '0';

			--mantsize;
		}

		fraction = 1.0e9 * frac1 + frac2;
	}

	/*
	 * Skim off the exponent.
	 */

	p = pexp;

	if (*p == 'E' || *p == 'e') {
		++p;

		if (*p == '-') {
			exp_sign = TRUE;
			++p;
		} else if (*p == '+') {
			++p;
		}

		if (ISDIGIT(*p)) {
			do {
				exp = exp * 10 + *p++ - '0';
			} while (ISDIGIT(*p));
		} else {
			p = pexp;
			goto done;
		}
	}

	if (exp_sign) {
		exp = fracexp - exp;
	} else {
		exp = fracexp + exp;
	}

	/*
	 * Generate a floating-point number that represents the exponent.
	 * Do this by processing the exponent one bit at a time to combine
	 * many powers of 2 of 10. Then combine the exponent with the
	 * fraction.
	 */

	if (exp < 0) {
		exp_sign = TRUE;
		exp      = -exp;
	} else {
		exp_sign = FALSE;
	}

	if (exp > max_exponent) {
		exp = max_exponent;

#ifdef USE_ERRNO
		errno = ERANGE;
#endif

    }

	dblexp = 1.0;

	for (d = powers_of_10; exp != 0; exp /= 2, ++d) {
		if ((exp & 01) != 0) {
			dblexp *= *d;
		}
	}

	if (exp_sign) {
		fraction /= dblexp;
	} else {
		fraction *= dblexp;
	}

done:
	if (endptr != NULL) {
		*endptr = (char*)p;
	}

	if (sign) {
		fraction = -fraction;
	}

	return fraction;
}
