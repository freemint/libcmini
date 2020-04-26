/*
 * ctype.h
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#ifndef _CTYPE_H_
#define _CTYPE_H_

int toupper(int c);
int tolower(int c);
int toascii(int c);
int isalnum(int c);
int isalpha(int c);
int isascii(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int isblank(int c);

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
#define isalpha(c)	(isupper(c) || islower(c))
#define iscntrl(c)  ((unsigned char)(c) < ' ' || (unsigned char)(c) >= 0x7f)
#define isgraph(c)  ((unsigned char)(c) > ' ' && (unsigned char)(c) != 0x7f)
#define isprint(c)  ((unsigned char)(c) >= ' ' && (unsigned char)(c) != 0x7f)
#define ispunct(c)  (!((unsigned char)(c) <= ' ' || (unsigned char)(c) == 0x7f  || \
                       ((unsigned char)(c) >= '0' && (unsigned char)(c) <= '9') || \
                       ((unsigned char)(c) >= 'A' && (unsigned char)(c) <= 'Z') || \
                       ((unsigned char)(c) >= 'a' && (unsigned char)(c) <= 'z')    \
                      )                                                            \
                    )
#define isxdigit(c) (isdigit(c) || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))
#define tolower(c)	(isupper(c) ? ((c) + 'a' - 'A') : (c))
#define toupper(c)	(islower(c) ? ((c) - 'a' + 'A') : (c))

#define isspace(c)  (((c) == ' ') || ((c) == '\t') || ((c == '\n')) || ((c) == '\r') || ((c) == '\v') || ((c) == '\f'))
#define isalnum(c)  (isalpha(c) || isdigit(c))

#endif /* CTYPE_H_ */
