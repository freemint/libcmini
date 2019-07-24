#include "lib.h"

/* globals */

short _app = 1;
unsigned long _PgmSize;
BASEPAGE *_base=0;
/* BasPag for libcmini compatibility */
extern BASEPAGE *BasPag __attribute__((alias("_base")));

int errno;

char **environ;
long __libc_argc = 1;
long __has_no_ssystem = 1;

static char *__libc_argv_default[] = { "unknown application", NULL };
char **__libc_argv = __libc_argv_default;

static FILE _StdInF =  { _IOMAGIC, (void *)0, EOF, NULL, { 1, 0 }, 0, 0 };
static FILE _StdOutF = { _IOMAGIC, (void *)1, EOF, NULL, { 0, 1 }, 0, 0 };
static FILE _StdErrF = { _IOMAGIC, (void *)2, EOF, NULL, { 0, 1 }, 0, 0 };

FILE *stdin = &_StdInF;
FILE *stdout = &_StdOutF;
FILE *stderr = &_StdErrF;
