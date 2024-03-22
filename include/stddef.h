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

#ifndef NULL
# ifndef __cplusplus
#  define NULL  ((void *)0)
# else
#  if __cplusplus < 201103L
#   define NULL  0
#  else
#   define NULL  nullptr
#  endif
# endif
#endif

/* Offset of member MEMBER in a struct of type TYPE.  */
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#endif /* STDDEF_H_ */
