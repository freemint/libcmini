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
extern char *strrchr(const char *s, int c);
extern size_t strnlen(const char* s, size_t maxlen);
extern char* strtok(char* s1, const char* s2);
char *strtok_r(char *s, const char *delim, char **scanpoint);
extern char* stpcpy(char* dest, const char* src);
extern char* stpncpy(char* dest, const char* src, size_t n);
extern size_t strlcpy(char* dst, const char* src, size_t siz);
extern size_t strlcat(char* dst, const char* src, size_t siz);
extern char* strsep(char** s, const char* sep);
extern char* strchrnul(const char* s, int c_in);
extern char* strcasestr(const char* s, const char* wanted);

extern void bzero(void *s, size_t n);
extern void bcopy(const void *src, void *dst, size_t n);
extern void *memset(void *b, int c, size_t len);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memmove(void *dest, const void *src, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t n);
extern void *memchr(const void *ptr, int ch, size_t count);
void *memrchr(const void *s, int ucharwanted, size_t size);
extern void* memccpy(void* dst, const void* src, int ucharstop, size_t size);
extern void* mempcpy(void* dstpp, const void* srcpp, size_t len);

extern char *strerror(int errnum);
extern char* strerror_r(int errnum, char *buf, size_t bufsize);

extern char *strrev(char *string);
extern char *strlwr(char *str);
extern char *strupr(char *str);

extern char* basename(const char *filename);
extern char* dirname(char *filename);

int strcoll(const char* __s1, const char* __s2);
size_t strxfrm(char* __dest, const char* __src, size_t __n);

#endif /* _STRING_H_ */
