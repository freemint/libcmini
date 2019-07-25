/*
 * main.c
 *
 *  Created on: 29.05.2013
 *      Author: mfro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mintbind.h>
#include "lib.h"

extern void _monstartup(void *, void *);
extern int main (int, char **, char **);
void _main(int, char *[], char *[]);
void __main();

static long parseargs(BASEPAGE *bp);

extern long _stksize;
extern long _initial_stack;

/* globals */

void *_heapbase = 0;
short _app = 1;
short _split_mem = 0;
long __has_no_ssystem = 1;
unsigned long _PgmSize;
BASEPAGE *_base=0;
/* BasPag for libcmini compatibility */
extern BASEPAGE *BasPag __attribute__((alias("_base")));

int errno;

char **environ;
long __libc_argc = 1;
static char *__libc_argv_default[] = { "unknown application", NULL };
char **__libc_argv = __libc_argv_default;

char *program_invocation_name = "unknown application";
char *program_invocation_short_name = "unknown application";

long _pdomain;		/* Error code of Pdomain call */

void _acc_main(void) {
	static char *acc_argv[] = { "", NULL }; /* no name and no arguments */

	if (_stksize == 0 || _stksize == -1L)
		_stksize = MINKEEP;

	if (_stksize < 0)
		_stksize = -_stksize;

//	if ((s = getenv("STACKSIZE")) != 0)
//		_stksize = atoi(s);

	/* stack on word boundary */
	_stksize &= 0xfffffffeL;

	if (_heapbase == 0) {
		_heapbase = (void *)Malloc(_stksize);
	}
	_setstack((char *) _heapbase + _stksize);

	/* this is an accessory */
	_app = 0;

	_main(1L, acc_argv, acc_argv);
	/*NOTREACHED*/
}
#undef isspace
#define isspace(c) ((c) == ' '||(c) == '\t')
#undef isdigit
#define isdigit(c) ((c) >= '0' && (c) <= '9')

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

void _crtinit(void) {
	extern void etext();	/* a "function" to fake out pc-rel addressing */

	register BASEPAGE *bp;
	register long m;
	register long freemem;

	/* its an application */
	_app = 1;

	bp = _base;

	/* m = # bytes used by environment + args */
	m = parseargs(bp);

	/* make m the total number of bytes required by program sans stack/heap */
	m += (bp->p_tlen + bp->p_dlen + bp->p_blen + sizeof(BASEPAGE));
	m = (m + 3L) & (~3L);

	/* freemem the amount of free mem accounting for MINFREE at top */
	if ((freemem = (long)bp->p_hitpa - (long)bp - MINFREE - m) <= 0L)
	    goto notenough;

	if (_initial_stack) {
	    /* the primary use of _initial_stack will be in dumping */
	    /* applications where only a heap for malloc makes sense */
	    _heapbase = (void *)((long)bp + m);
	    _stksize = _initial_stack;
	} else {
		if (_stksize >= -1L)
			/* malloc from Malloc first, then from own heap */
			_split_mem = 1;
	}

	if (_stksize == -1L) {
		_stksize = freemem;
		_heapbase = (void *)((long)bp + m);
	} else if (_stksize == 0L) {	/* free all but MINKEEP */
		_stksize = MINKEEP;
	} else if (_stksize == 1L) { 	/* keep 1/4, free 3/4 */
		_stksize = freemem >> 2;
	} else if (_stksize ==  2L) {	/* keep 1/2, free 1/2 */
		_stksize = freemem >> 1;
	} else if (_stksize == 3L) {	/* keep 3/4, free 1/4 */
		_stksize = freemem - (freemem >> 2);
	} else {
		if(_stksize < -1L) {	/* keep |_stksize|, use heap for mallocs */
			_stksize = -_stksize;
			_heapbase = (void *)((long)bp + m);
		}
	}
/*
	if ((s = getenv("STACKSIZE")) != 0)
		_stksize = atoi(s);
*/
	/* make m the total number of bytes including stack */
	_stksize = _stksize & (~3L);
	m += _stksize;

	/* make sure there's enough room for the stack */
	if (((long)bp + m) > ((long)bp->p_hitpa - MINFREE))
	    goto notenough;

	/* set up the new stack to bp + m  */
	_setstack((char *)bp + m);

	/* shrink the TPA */
	(void)Mshrink(bp, m);

	/* keep length of program area */
	_PgmSize = m;

	/* establish handlers,  call the main routine */
/*	_init_signal(); */

	/* start profiling, if we were linked with gcrt0.o */
	_monstartup((void *)bp->p_tbase, (void *)((long)etext - 1));

	_main(__libc_argc, __libc_argv, environ);
	/* not reached normally */

notenough:
	(void) Cconws("Fatal error: insufficient memory\r\n");
	(void) Cconws("Hint: either decrease stack size using 'stack' command (not recomended)\r\n" \
		   "      or increase TPA_INITIALMEM value in mint.cnf.\r\n");
	Pterm(-1);
}

/*
 * parseargs(bp): parse the environment and arguments pointed to by the
 * basepage. Return the number of bytes of environment and arguments
 * that have been appended to the bss area (the environ and argv arrays
 * are put here, as is a temporary buffer for the command line, if
 * necessary).
 *
 * The MWC extended argument passing scheme is assumed.
 *
 */
static long parseargs(BASEPAGE *bp) {
	long count = 4;		/* compensate for aligning */
	long  i;
	char *from, *cmdln, *to;
	char **envp, **arg;
	char *null_list = 0;
	/* flag to indicate desktop-style arg. passing */
	long desktoparg;

	/* handle the environment first */

	environ = envp = (char **)(( (long)bp->p_bbase + bp->p_blen + 4) & (~3));
	from = bp->p_env;
	while (*from) {

/* if we find MWC arguments, tie off environment here */
		if (*from == 'A' && *(from+1) == 'R' && *(from+2) == 'G' &&
		    *(from+3) == 'V' && *(from+4) == '=')
		{
			*envp++ = (char *) 0; count += 4;
			*from = 0;
#ifdef STRICTLY_COMPATIBLE_WITH_STANDARD
			if (bp->p_cmdlin[0] != 127)
				goto old_cmdlin;
#endif
			from += 5;        /* skip ARGV= string */

			/* find list of empty params
			 */
			if (*from == 'N' && *(from+1) == 'U'
			    && *(from+2) == 'L' && *(from+3) == 'L'
			    && *(from+4) == ':')
			{
				null_list = from + 5;
			}

			while (*from++) ; /* skip ARGV= value */
			__libc_argv = arg = envp;
			*arg++ = from; count+= 4;
			while (*from++) ; /* skip __libc_argv[0] */
			goto do_argc;
		}
		*envp++ = from;
		count += 4;
		desktoparg = 1;
		while (*from) {
			if (*from == '=') {
				desktoparg = 0;
			}
			from++;
		}
		from++;		/* skip 0 */

/* the desktop (and some shells) use the environment in the wrong
   way, putting in "PATH=\0C:\0" instead of "PATH=C:". so if we
   find an "environment variable" without an '=' in it, we
   see if the last environment variable ended with '=\0', and
   if so we append this one to the last one
 */
		if(desktoparg && envp > &environ[1])
		{
		/* launched from desktop -- fix up env */
		    char *p, *q;

		    q = envp[-2];	/* current one is envp[-1] */
		    while (*q) q++;
		    if (q[-1] == '=') {
			p = *--envp;
			while(*p)
			   *q++ = *p++;
		        *q = '\0';
		   }
		}
	}
	*envp++ = (char *)0;
	count += 4;

#ifdef STRICTLY_COMPATIBLE_WITH_STANDARD
old_cmdlin:
#endif
/* Allocate some room for the command line to be parsed */
	cmdln = bp->p_cmdlin;
	i = *cmdln++;
	from = to = (char *) envp;
	if (i > 0) {
		count += (i&(~3));
		envp = (char **) ( ((long) envp)  + (i&(~3)) );
	}
	envp += 2; count += 8;

/* Now parse the command line and put __libc_argv after the environment */

	__libc_argv = arg = envp;
	*arg++ = "";		/* __libc_argv[0] not available */
	count += 4;
	while(i > 0 && isspace(*cmdln) )
		cmdln++,--i;

	/*
	 * MagXDesk only uses ARGV if the arg is longer than the 126 character
	 * of bp->cmdlin. If the arg is short enough and contains a file name with
	 * blanks it will be come quoted via bp->cmdlin!!
	*/
	if (cmdln[0] != '\'')
	{
		while (i > 0) {
			if (isspace(*cmdln)) {
				--i; cmdln++;
				while (i > 0 && isspace(*cmdln))
					--i,cmdln++;
				*to++ = 0;
			}
			else {
				if ((*to++ = *cmdln++) == 0) break;
				--i;
			}
		}
	}
	else
	{
		int in_quote = 0;

		while (i > 0)
		{
			if (*cmdln == '\'')
			{
				i--;
				cmdln++;
				if (in_quote)
				{
					if (*cmdln == '\'')		/* double ': file name contains ' */
					{
						*to++ = *cmdln++;
						i--;
					}
					else
					{
						in_quote = 0;
						*to++ = 0;				/* end of quoted arg */
						i--; cmdln++;
					}
				}
				else
					in_quote = 1;
			}
			else
			{
				if (*cmdln == ' ')
				{
					if (in_quote)
					{
						*to++ = *cmdln++;
						i--;
					}
					else
					{
						--i; cmdln++;
						*to++ = 0;
					}
				}
				else
				{
					*to++ = *cmdln++;
					i--;
				}
			}
		}
	}

	*to++ = '\0';
	*to = '\0'; /* bug fix example:cmdln == '\3' 'a' ' ' 'b' '\0' */
	/* the loop below expects \0\0 at end to terminate! */
	/* the byte @ cmdln[i+2] != 0 when fast bit is set */
do_argc:
	/* Find out the name we have been invoked with.  */

 	if (__libc_argv[0] != 0 && __libc_argv[0][0] != '\0') {
 	  char* p;
 	  program_invocation_name = __libc_argv[0];
 	  p = strrchr (program_invocation_name, '\\');
 	  if (p == 0)
 	    p = strrchr (program_invocation_name, '/');
 	  if (p != 0)
 	    p++;
 	  program_invocation_short_name = p == 0 ?
 	      program_invocation_name : p;
 	}

	__libc_argc = 1;		/* at this point __libc_argv[0] is done */
	while (*from) {
		*arg++ = from;
		__libc_argc++;
		count += 4;
		while(*from++) ;
	}
	*arg = (char *) 0;

	/* zero epmty params
	 */
	if (null_list) {
		char *s;
		long idx;

		while (*null_list) {
			s = null_list;

			while (* ++null_list) {	/* find ',' or '\0' */
				if (*null_list == ',') {
					*null_list++ = 0;
					break;
				}
			}

			idx = 0;
			for (;;) {
				if (! isdigit(*s))
					goto bail_out;

				/* don't feed this to strtol(),
				 * do the ascii -> long conversion by
				 * hand for efficency
				 */
				idx += *s++ - '0';
				if (*s)
					idx = (idx << 3) + (idx << 1);
				else
					break;
			}

			if (idx < __libc_argc)
				*(__libc_argv[idx]) = 0;
			else
				goto bail_out;
		}
	}

bail_out:
	return count+4;
}


FILE _StdOutF;
FILE _StdInF;
FILE _StdErrF;
FILE _StdAuxF;
FILE _StdPrnF;

FILE *stdin;
FILE *stdout;
FILE *stderr;


void _main (int _argc, char **_argv, char **_envp) {
	if (_app)
		_pdomain = Pdomain(1);	/* set MiNT domain */

	/* if stderr is not re-directed to a file, force 2 to console
	 * (UNLESS we've been run from a shell we trust, i.e. one that supports
	 *  the official ARGV scheme, in which case we leave stderr be).
	 */
	if(!*_argv[0] && isatty (2))
		(void) Fforce(2, -1);

	stdin = &_StdInF; 				// _StdInF is in .bss and cleared from os
	bzero(stdin, sizeof(FILE));	// but we cleared out again - safe is safe (paranoia???)
	FILE_SET_HANDLE(stdin, 0);
	stdin->__pushback = EOF;
	stdin->__mode.__read = 1;

	// While testing I experienced that there is
	// always a CR in the standard input buffer
	if (Cconis()) {
		Cnecin();
	}

	stdout = &_StdOutF;
	bzero(stdout, sizeof(FILE));
	FILE_SET_HANDLE(stdout, 1);
	stdout->__pushback = EOF;
	stdin->__mode.__write = 1;

	stderr = &_StdErrF;
	bzero(stderr, sizeof(FILE));
	FILE_SET_HANDLE(stderr, 2);
	stderr->__pushback = EOF;
	stdin->__mode.__write = 1;

	exit(main(_argc, _argv, _envp));
}

