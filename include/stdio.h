/*
 * stdio.h
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdlib.h>
#include <stdarg.h>
#include <osbind.h>

enum
{
    _IOFBF,             /* stream is fully buffered */
    _IOLBF,             /* stream is line buffered */
    _IONBF              /* stream is unbuffered */
};

typedef struct __stdio_file FILE;
typedef struct __stdio_file
{
    int __magic;
#define	_IOMAGIC ((int) 0xfedabeeb)	/* Magic number to fill `__magic'.  */

    void *BufPtr;       /* next byte write */
    void *BufLvl;       /* next byte read */
    void *BufStart;     /* first byte of buffer */
    void *BufEnd;       /* first byte after buffer */

    long Handle;        /* GEMDOS handle */
#if 0
    char Flags;
    char resv;
    char ChrBuf;
    char ungetFlag;
#endif
    FILE *__next;       /* Next FILE in the linked list.  */

} FILE;

extern FILE *stdout;
extern FILE *stdin;
extern FILE *stderr;

#define stdaux	(&_StdAuxF)
#define stdprn	(&_StdPrnF)

extern FILE _StdOutF;
extern FILE _StdInF;
extern FILE _StdErrF;
extern FILE _StdAuxF;
extern FILE _StdPrnF;

#ifndef NULL
#define NULL        ((void *)0)
#endif /* NULL */

#define OPEN_MAX        32
#define FOPEN_MAX       32
#define FILENAME_MAX    128
#define PATH_MAX        128
#define BUFSIZ          1024
#define EOF             (-1)

/*
 * access modes for open()
 */
enum
{
    O_RDONLY = 0x00,            /* open read-only */
    O_WRONLY = 0x01,            /* open write-only (this doesn't work apparently on _all_ TOS versions */
    O_RDWR = 0x02,              /* open for reading and writing */
    O_APPEND = 0x08,            /* position file pointer to end of file before each write */
    O_CREAT = 0x20,             /* create file if it doesn't exist */
    O_TRUNC = 0x40,             /* truncate to zero size */
    O_EXCL = 0x80               /* ? */
};

/*
 * these are #defined in osbind.h
 *
 * we rather do enums - compiler indicates clashing identifiers
 */
#ifdef SEEK_SET
#undef SEEK_SET
#endif
#ifdef SEEK_CUR
#undef SEEK_CUR
#endif
#ifdef SEEK_END
#undef SEEK_END
#endif

enum
{
    SEEK_SET = 0,               /* Seek from beginning of file.  */
    SEEK_CUR = 1,               /* Seek from current position.  */
    SEEK_END = 2                /* Seek from end of file.  */
};

extern int errno;

FILE *fopen(const char *path, const char *mode);
int fclose(FILE *fp);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int fseek(FILE *fp, long offset, int origin);
long ftell(FILE *stream);
int fflush(FILE *stream);



int fputs(const char *s, FILE *stream);

int puts(const char *s);

int fputc(int c, FILE *stream);

int putc(int c, FILE *stream);

int scanf(const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int fgetc(FILE *stream);
int putchar(int c);
int fprintf(FILE *stream, const char *format, ...);
int vfprintf(FILE *stream, const char *format, va_list ap);

int printf(const char *fmt, ...);
int snprintf(char *s, size_t size, const char *fmt, ...);
int vsnprintf(char *str, size_t size, const char *fmt, va_list va);
int sprintf(char *s, const char *format, ...);
int vsprintf(char *s, const char *format, va_list va);

int puts(const char *s);

int open(const char *filename, int access, ...);
int close(int fd);
int unlink(const char *filename);

static inline int fileno(FILE *stream) { return stream->Handle; }
int getc(FILE *stream);
#endif /* STDIO_H_ */
