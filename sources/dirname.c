/*  dirname.c -- MiNTLib.
    Copyright (C) 1999 Guido Flohr <guido@freemint.de>

    This file is part of the MiNTLib project, and may only be used
    modified and distributed under the terms of the MiNTLib project
    license, COPYMINT.  By continuing to use, modify, or distribute
    this file you indicate that you have read the license and
    understand and accept it fully.
*/

#include <string.h>
#ifdef MINTLIB_COMPATIBLE
#   include <ctype.h>
#else
#   include "../include/ctype.h"
#endif

#include "lib.h"


#define ISUPPER(c)  ((c) >= 'A' && (c) <= 'Z')
#define ISLOWER(c)  ((c) >= 'a' && (c) <= 'z')
#define ISDIGIT(c)  ((c) >= '0' && (c) <= '9')


char*
dirname(char* path)
{
	static char* here = ".";

	*here = '.';

	if (path == NULL) {
		path = here;  /* Yes, this is stupid.  See below!  */
	} else if (path[1] == ':' && path[2] == '\0' && (ISUPPER(path[0]) || ISLOWER(path[0]) || ISDIGIT(path[0]))) {
		/* A drive letter followed by a colon.  */
	} else {
		char* last_slash = strrchr (path, '\\');

		/* End of Atari-specifific kludges.  The rest is more or less taken
		   unchanged from the GNU libc.  */
		if (last_slash == path) {
			/* The last slash is the first character in the string.  We have to
			   return "/".  */
			++last_slash;
		} else if (last_slash != NULL && last_slash[1] == '\0') {
			/* The '/' or '\\' is the last character, we have to look further.  */
			char* maybe_last_slash = memchr(path, last_slash - path, '/');

			if (maybe_last_slash == NULL) {
				maybe_last_slash = memchr (path, last_slash - path, '\\');
			}

			last_slash = maybe_last_slash;
		}

		if (last_slash != NULL) {
			/* Terminate the path.  */
			*last_slash = '\0';
		} else {
			/* This assignment is ill-designed but the XPG specs require to
			   return a string containing "." in any case no directory part is
			   found and so a static and constant string is required.  */
			path = here;
		}
	}

	return path;
}
