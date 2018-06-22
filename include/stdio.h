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
#include <mint/osbind.h>

/* The mode of I/O, as given in the MODE argument to fopen, etc.  */
typedef struct
{
  unsigned int __read:1;    /* Open for reading.  */
  unsigned int __write:1;   /* Open for writing.  */
  unsigned int __append:1;  /* Open for appending.  */
  unsigned int __binary:1;  /* Opened binary.  */
  unsigned int __create:1;  /* Create the file.  */
  unsigned int __exclusive:1;   /* Error if it already exists.  */
  unsigned int __truncate:1;    /* Truncate the file on opening.  */
} __io_mode;

typedef struct __stdio_file FILE;
struct __stdio_file
{
    long __magic;
#define	_IOMAGIC (0xfedabeecL)	/* Magic number to fill `__magic'.  */

#if 0
    char *__bufp;     /* next byte write */
    void *__buflvl;   /* next byte read */
    void *__bufstart; /* first byte of buffer */
    void *__bufend;   /* first byte after buffer */
#endif
    /* long Handle; */
    long __hdl;       /* GEMDOS handle */
#if 0
    char __flags;
    char __resv;
    char __char_buf;
#endif
    int __pushback;
    FILE *__next;     /* Next FILE in the linked list.  */
    __io_mode __mode;     /* File access mode.  */
    unsigned int __eof;
    unsigned int __error;
};

/* All the known streams are in a linked list
   linked by the `next' field of the FILE structure.  */
extern FILE *__stdio_head;      /* Head of the list.  */

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
#endif

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
extern FILE *freopen(const char *path, const char *mode, FILE *stream);
extern int fclose(FILE *fp);
extern int fcloseall(void);
extern size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
extern size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
extern int fseek(FILE *fp, long offset, int origin);
extern long ftell(FILE *stream);
extern void rewind(FILE *stream);

int setvbuf(FILE *stream, char *buf, int mode, size_t size);

extern int fputs(const char *s, FILE *stream);
extern int puts(const char *s);
extern int fputc(int c, FILE *stream);
extern int putc(int c, FILE *stream);
extern int putchar(int c);
extern int puts(const char *s);

extern int fgetc(FILE *stream);
extern char* fgets(char *s, int n, FILE *stream);
extern int getc(FILE *stream);
/* No gets() anymore! */
#define getchar()  fgetc(stdin)
extern int ungetc(int c, FILE *stream);

extern int scanf(const char *format, ...);
extern int fscanf(FILE *stream, const char *format, ...);
extern int sscanf(const char *str, const char *format, ...);
extern int vscanf(const char *format, va_list list);
extern int vfscanf(FILE* fp, const char *format, va_list list);
extern int vsscanf(const char* str, const char *format, va_list list);

extern int fprintf(FILE *stream, const char *format, ...);
extern int vfprintf(FILE *stream, const char *format, va_list ap);
extern int printf(const char *fmt, ...);
extern int vprintf(const char *fmt, va_list ap);
extern int snprintf(char *s, size_t size, const char *fmt, ...);
extern int vsnprintf(char *str, size_t size, const char *fmt, va_list va);
extern int sprintf(char *s, const char *format, ...);
extern int vsprintf(char *s, const char *format, va_list va);
extern int asprintf(char** strp, const char* format, ...);
extern int vasprintf(char** strp, const char* format, va_list ap);

extern int fflush(FILE *stream);
extern int feof(FILE *stream);
extern int ferror(FILE *stream);
extern void clearerr(FILE *stream);
extern int fileno(FILE *stream);

extern int open(const char *filename, int access, ...);
extern int close(int fd);
extern int unlink(const char *filename);

extern int remove(const char *filename);
extern int rename(const char *oldname, const char *newname);

#endif /* STDIO_H_ */
