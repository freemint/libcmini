/*
 * stddef.h
 *
 *  Created on: 31.05.2013
 *      Author: mfro
 */

#ifndef STDDEF_H_
#define STDDEF_H_

typedef unsigned long size_t;
typedef signed long ptrdiff_t;

#ifdef __NULL
#undef __NULL
#endif
#define __NULL (0L)

/* Offset of member MEMBER in a struct of type TYPE.  */
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#endif /* STDDEF_H_ */
