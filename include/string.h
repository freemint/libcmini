/*
 * string.h
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#ifndef _STRING_H_
#define _STRING_H_

#include <stdlib.h>

extern char *strcat(char *dst, const char *src);
extern char *strcpy(char *dst, const char *src);
extern size_t strlen(const char *s);
extern char *strncat(char *dst, const char *src, size_t max);
extern int strcmp(const char *s1, const char *s2);
extern int stricmp(const char *s1, const char *s2);
extern int strcasecmp(const char *s1, const char *s2);
extern int strncmp(const char *s1, const char *s2, size_t max);
extern int strnicmp(const char *s1, const char *s2, size_t max);
extern int strncasecmp(const char *s1, const char *s2, size_t max);
extern char *strncpy(char *dst, const char *src, size_t max);
extern char *strdup(const char *s1);
extern char *strndup(const char *s1, size_t n);
extern char *strchr(const char *s, int c);
extern char *strstr(const char *s, const char *wanted);
extern size_t strspn(const char *s, const char *accept);
extern size_t strcspn(const char *s, const char *reject);
extern char *strpbrk(const char *s, const char *breakat);
extern char *index(const char *s, int c);
extern char *strrchr(const char *s, int c);
extern char *rindex(const char *s, int c);

extern void bzero(void *s, size_t n);
extern void bcopy(const void *src, void *dst, size_t n);
extern void *memset(void *b, int c, size_t len);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memmove(void *dest, const void *src, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t n);
void *memchr(const void *ptr, int ch, size_t count);

extern char *strerror(int errnum);
extern char* strerror_r(int errnum, char *buf, size_t bufsize);

extern char *strrev(char *);
extern char *strlwr(char *);
#endif /* _STRING_H_ */
