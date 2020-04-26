/*
 * lib.h
 *
 *  Created on: 08.06.2013
 *      Author: mfro
 */

#ifndef LIB_H_
#define LIB_H_

#include <stdio.h>
#include <stdarg.h>

/* definitions needed for stack stuff */

#define MINFREE	(8L * 1024L)		/* free at least this much mem on top */
#define MINKEEP (64L * 1024L)		/* keep at least this much mem on stack */
void _setstack(char *);

/* definitions needed in malloc.c and realloc.c */

struct mem_chunk
{
    long valid;
#define VAL_FREE  0xf4ee0abcL
#define VAL_ALLOC 0xa11c0abcL
#define VAL_BORDER 0xb04d0abcL

    struct mem_chunk *next;
    unsigned long size;
};
#define ALLOC_SIZE(ch) (*(long *)((char *)(ch) + sizeof(*(ch))))
#define BORDER_EXTRA ((sizeof(struct mem_chunk) + sizeof(long) + 7) & ~7)

/* linked list of free blocks */
extern struct mem_chunk _mchunk_free_list;

void __mallocChunkSize(size_t siz);


/* definitions needed in FILE related functions (fopen, fseek etc.) */

#ifdef __MINTLIB_COMPATIBLE
#	define FILE_SET_HANDLE(fp, H) fp->__cookie = (void*)H
#	define FILE_GET_HANDLE(fp) ((long)fp->__cookie)
#else
#	define FILE_SET_HANDLE(fp, H) fp->__hdl = H
#	define FILE_GET_HANDLE(fp) (fp->__hdl)
#endif

typedef void (*ExitFn)(void);
extern char **environ;
extern short _app;
extern unsigned long _PgmSize;
extern long __libc_argc;
extern char **__libc_argv;

struct _mem_stream { char *xstring; char *xestring; };

int doprnt(int (*)(int, void *), void *stream, const char *fmt, va_list va);
int doscan(int (*getchar_func)(void*), int (*ungetchar_func)(int, void*), void* stream, const char* fmt, va_list va);
int __addchar(int c, void *stream);
int unx2dos(const char *unx, char *dos);
int dos2unx(const char *dos, char *unx);
unsigned long __strtoul_internal(const char *nptr, char **endptr, int base, int *sign);
unsigned long long __strtoull_internal(const char *nptr, char **endptr, int base, int *sign);
void _acc_main(void);
void _crtinit(void);
void _crtinit_noargs(void);

#undef __set_errno
#define __set_errno(e) (errno = (int)(e))

#ifndef NO_CONST
#  ifdef __GNUC__
#    define NO_CONST(p) __extension__({ union { const void *cs; void *s; } x; x.cs = p; x.s; })
#  else
#    define NO_CONST(p) ((void *)(p))
#  endif
#endif

#endif /* LIB_H_ */
