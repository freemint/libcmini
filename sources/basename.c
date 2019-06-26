/*  basename.c -- MiNTLib.
    Copyright (C) 1999, 2000 Guido Flohr <guido@freemint.de>

    This file is part of the MiNTLib project, and may only be used
    modified and distributed under the terms of the MiNTLib project
    license, COPYMINT.  By continuing to use, modify, or distribute
    this file you indicate that you have read the license and
    understand and accept it fully.
*/

#include <string.h>


char*
basename(const char* filename)
{
	const char* backslash = strrchr(filename, '\\');

	if (backslash != NULL) {
		filename = backslash + 1;
	}

	return (char*)filename;
}
