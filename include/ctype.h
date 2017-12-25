/*
 * ctype.h
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#ifndef _CTYPE_H_
#define _CTYPE_H_

#define isdigit(c)	(((c) >= '0') && ((c) <= '9'))
#define isupper(c)	((c) >= 'A' && ((c) <= 'Z'))
#define islower(c)	((c) >= 'a' && ((c) <= 'z'))
#define isalpha(c)	(isupper((c)) || islower(c))
#define tolower(c)	(isupper(c) ? ((c) + 'a' - 'A') : (c))
#define toupper(c)	(islower(c) ? ((c) - 'a' - 'A') : (c))

#define isspace(c)  (((c) == ' ') || ((c) == '\t') || ((c == '\n')) || ((c) == '\r') || ((c) == '\v'))

#endif /* CTYPE_H_ */
