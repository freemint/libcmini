#include "lib.h"

#ifdef __GNUC__

FILE *__stdio_head;

static FILE _StdInF =  { _IOMAGIC, (void *)0, EOF, NULL, { 1, 0 }, 0, 0 };
static FILE _StdOutF = { _IOMAGIC, (void *)1, EOF, NULL, { 0, 1 }, 0, 0 };
static FILE _StdErrF = { _IOMAGIC, (void *)2, EOF, NULL, { 0, 1 }, 0, 0 };

FILE *stdin = &_StdInF;
FILE *stdout = &_StdOutF;
FILE *stderr = &_StdErrF;

#endif
