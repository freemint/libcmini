/*
 * stdlib.h
 *
 *  Created on: 31.05.2013
 *      Author: mfro
 */

#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stddef.h>

extern int atoi(const char *c);
extern long atol(const char *c);
extern double atof(const char *c);
extern void *malloc(size_t size);
extern void *calloc(size_t nmemb, size_t size);
extern void *realloc(void *ptr, size_t size);
extern void free(void *ptr);

extern char *ltoa(long value, char *buffer, int radix);
#define _ltoa(a, b, c) ltoa((a), (b), (c))

extern char *getenv(const char *name);
extern int setenv(const char *name, const char *value, int replace);
extern int unsetenv(const char *name);

extern int atexit(void (*func)(void));
extern void exit(int status);
extern void abort(void);

#ifdef __MSHORT__
#define	RAND_MAX (0x7FFF)
#else
#define	RAND_MAX (0x7FFFFFFFL)
#endif

extern int rand(void);
extern void srand(unsigned int seed);
extern long lrand(void);
extern void srand48(long int seed);

extern long strtol(const char*, char**, int);
extern unsigned long strtoul(const char*, char**, int);
extern long long strtoll(const char*, char**, int);
extern unsigned long long strtoull(const char*, char**, int);
extern double strtod(const char* s, char** endp);

extern void* bsearch(const void* key, const void* base, size_t num, size_t size, int (*cmp)(const void*, const void*));
extern void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));

extern int abs(int n);
extern long labs(long n);

#ifndef NULL
# define NULL	((void *) 0)
#endif /* !defined NULL */

#endif /* _STDLIB_H_ */
