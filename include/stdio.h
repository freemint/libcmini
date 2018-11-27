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

FILE *stdout;
FILE *stdin;
FILE *stderr;

#define stdaux	(&_StdAuxF)
#define stdprn	(&_StdPrnF)

FILE _StdOutF;
FILE _StdInF;
FILE _StdErrF;
FILE _StdAuxF;
FILE _StdPrnF;

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

int errno;

FILE *fopen(const char *path, const char *mode);
int fclose(FILE *fp);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int fseek(FILE *fp, long offset, int origin);
long ftell(FILE *stream);




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
