/*
 * ctype.h
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#ifndef _CTYPE_H_
#define _CTYPE_H_

extern unsigned int* _ctype;

#define _IScntrl    0x01        /* control character */
#define _ISdigit    0x02        /* numeric digit */
#define _ISupper    0x04        /* upper case */
#define _ISlower    0x08        /* lower case */
#define _ISspace    0x10        /* whitespace */
#define _ISpunct    0x20        /* punctuation */
#define _ISxdigit   0x40        /* hexadecimal */
#define _ISblank    0x80        /* blank */
#define _ISgraph    0x100       /* graph */
#define _ISprint    0x200       /* print */

#define isdigit(c)	(((c) >= '0') && ((c) <= '9'))
#define isupper(c)	((c) >= 'A' && ((c) <= 'Z'))
#define islower(c)	((c) >= 'a' && ((c) <= 'z'))
#define isalpha(c)	(isupper((c)) || islower(c))
#define tolower(c)	(isupper(c) ? ((c) + 'a' - 'A') : (c))
#define toupper(c)	(islower(c) ? ((c) - 'a' - 'A') : (c))

#define isspace(c)  (((c) == ' ') || ((c) == '\t') || ((c == '\n')) || ((c) == '\r') || ((c) == '\v'))
#define isalnum(c)  (isalpha(c) || isdigit(c))

#endif /* CTYPE_H_ */
