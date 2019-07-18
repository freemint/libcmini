#ifndef _SETJMP_H_
#define _SETJMP_H_

typedef struct
{
	long retaddr;
#ifndef __FASTCALL__
	long regs[11];
#else
	long regs[10];
#endif
	double fpregs[8];
} jmp_buf;

int setjmp(jmp_buf *buf)
{
	__asm__ __volatile__(
#ifndef __FASTCALL__
		"	movem.l	%%d2-%%d7/%%a2-%%a7,(%[regs])	\n\t"
#else
		"	movem.l	%%d3-%%d7/%%a2-%%a7,(%[regs])	\n\t"
#endif
		:							/* output */
		: [regs] "a" (&buf->regs[0])	/* input */
		: "a0", "memory"
	);
	return 0;
}

void longjmp(jmp_buf *buf, int val)
{
	register int d0 __asm__("%d0") = val;
	register long *a0 __asm__("%a0") = &buf->regs[0];

	if (val == 0)	/* avoid infinite loop */
		val = 1;

	__asm__ __volatile__(
#ifndef __FASTCALL__
		"	movem.l	(%%a0),%%d2-%%d7/%%a2-%%a7	\n\t"
#else
		"	movem.l	(%%a0),%%d3-%%d7/%%a2-%%a7	\n\t"
#endif
		"	rts								\n\t"
		:							/* output */
		: "a" (a0), "d" (d0)
		: /* not reached; so no need to declare any clobbered regs */
	);
};

#endif /* _SETJMP_H_ */
