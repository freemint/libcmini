/*  strtok.c -- MiNTLib.
    Copyright (C) 2000 Guido Flohr <guido@freemint.de>

    This file is part of the MiNTLib project, and may only be used
    modified and distributed under the terms of the MiNTLib project
    license, COPYMINT.  By continuing to use, modify, or distribute
    this file you indicate that you have read the license and
    understand and accept it fully.
*/


#include <string.h>


static char* strtok_r(char* s, register const char* delim, char** scanpoint);

static char* save_ptr;


char*
strtok(char* s, const char* delim)
{
	return strtok_r(s, delim, &save_ptr);
}


char*       /* NULL if no token left */
strtok_r(char* s, register const char* delim, char** scanpoint)
{
	register char* scan;
	char*          tok;

	if (s == NULL && *scanpoint == NULL) {
		return NULL;
	} else if (s != NULL) {
		scan = s;
	} else {
		scan = *scanpoint;
	}

	/*
	 * Scan leading delimiters.
	 */
	while (*scan != '\0') {
		register const char *dscan;

		for (dscan = delim; *dscan != '\0'; ++dscan) {
			if (*scan == *dscan) {
				break;
			}
		}

		if (*dscan == '\0') {
			break;
		}

		++scan;
	}

	if (*scan == '\0') {
		*scanpoint = NULL;
		return NULL;
	}

	tok = scan;

	/*
	 * Scan token.
	 */
	while (*scan != '\0') {
		register const char *dscan = delim;

		while (*dscan != '\0') {  /* ++ moved down. */
			if (*scan == *dscan++) {
				*scanpoint = scan + 1;
				*scan      = '\0';

				return tok;
			}
		}

		++scan;
	}

	/*
	 * Reached end of string.
	 */
	*scanpoint = NULL;

	return tok;
}
