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

typedef struct __stdio_file FILE;
typedef struct __stdio_file
{
    int __magic;
#define	_IOMAGIC ((int) 0xfedabeeb)	/* Magic number to fill `__magic'.  */

#if 0
    void *BufPtr;     /* next byte write */
    void *BufLvl;     /* next byte read */
    void *BufStart;   /* first byte of buffer */
    void *BufEnd;     /* first byte after buffer */
#endif
    long Handle;      /* GEMDOS handle */
#if 0
    char Flags;
    char resv;
    char ChrBuf;
    char ungetFlag;
#endif
    FILE *__next;     /* Next FILE in the linked list.  */

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

#define _IONBF	2

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
#define O_RDONLY        0x00    /* open read-only */
#define O_WRONLY        0x01    /* open write-only (this doesn't work apparently on _all_ TOS versions */
#define O_RDWR          0x02    /* open for reading and writing */
#define O_APPEND        0x08    /* position file pointer to end of file before each write */
#define O_CREAT         0x20    /* create file if it doesn't exist */
#define O_TRUNC         0x40    /* truncate to zero size */
#define O_EXCL          0x80    /* ? */

#define SEEK_SET    0   /* Seek from beginning of file.  */
#define SEEK_CUR    1   /* Seek from current position.  */
#define SEEK_END    2   /* Seek from end of file.  */

extern int errno;

extern FILE *fopen(const char *path, const char *mode);
extern int fclose(FILE *fp);
extern size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
extern size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
extern int fseek(FILE *fp, long offset, int origin);
extern long ftell(FILE *stream);




extern int fputs(const char *s, FILE *stream);
extern int puts(const char *s);
extern int fputc(int c, FILE *stream);
extern int putc(int c, FILE *stream);

extern int scanf(const char *format, ...);
extern int fscanf(FILE *stream, const char *format, ...);
extern int fgetc(FILE *stream);
extern int putchar(int c);
extern int fprintf(FILE *stream, const char *format, ...);
extern int vfprintf(FILE *stream, const char *format, va_list ap);

extern int printf(const char *fmt, ...);
extern int snprintf(char *s, size_t size, const char *fmt, ...);
extern int vsnprintf(char *str, size_t size, const char *fmt, va_list va);
extern int sprintf(char *s, const char *format, ...);
extern int vsprintf(char *s, const char *format, va_list va);

extern int puts(const char *s);

extern int open(const char *filename, int access, ...);
extern int close(int fd);
extern int unlink(const char *filename);

// static inline int fileno(FILE *stream) { return stream->Handle; }
// static inline int getc(FILE *stream) { return fgetc(stream); }
#endif /* STDIO_H_ */
