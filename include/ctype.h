/*
 * ctype.h
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#ifndef CTYPE_H
#define CTYPE_H

#define _Isbit(bit) (1 << (bit))
enum
{
    _ISupper    = _Isbit(0),
    _ISlower    = _Isbit(1),
    _ISalpha    = _Isbit(2),
    _ISdigit    = _Isbit(3),
    _ISxdigit   = _Isbit(4),
    _ISspace    = _Isbit(5),
    _ISprint    = _Isbit(6),
    _ISgraph    = _Isbit(7),
    _ISblank    = _Isbit(8),
    _IScntrl    = _Isbit(9),
    _ISpunct    = _Isbit(10),
    _ISalnum    = _Isbit(11)
};


#define isdigit(c)	(((c) >= '0') && ((c) <= '9'))
#define isupper(c)	((c) >= 'A' && ((c) <= 'Z'))
#define islower(c)	((c) >= 'a' && ((c) <= 'z'))
#define isalpha(c)	(isupper((c)) || islower(c))
#define tolower(c)	(isupper(c) ? ((c) + 'a' - 'A') : (c))
#define toupper(c)	(islower(c) ? ((c) - 'a' - 'A') : (c))

#define isspace(c)  (((c) == ' ') || ((c) == '\t') || ((c == '\n')) || ((c) == '\r') || ((c) == '\v'))
#define isalnum(c)  (isalpha(c) || isdigit(c))

#endif /* CTYPE_H_ */
