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
		"	movem.l	d2-d7/a2-a7,(%[regs])	\n\t"
#else
		"	movem.l	d3-d7/a2-a7,(%[regs])	\n\t"
#endif
		:							/* output */
		: [regs] "a" (&buf->regs[0])	/* input */
		: "a0", "memory"
	);
	return 0;
}

void longjmp(jmp_buf *buf, int val)
{
	if (val == 0)	/* avoid infinite loop */
		val = 1;

	__asm__ __volatile__(
#ifndef __FASTCALL__
		"	movem.l	(%[regs]),d2-d7/a2-a7	\n\t"
#else
		"	movem.l	(%[regs]),d3-d7/a2-a7	\n\t"
#endif
		"	move.l	%[val],d0				\n\t"
		"	rts								\n\t"
		:							/* output */
		: [regs] "a" (&buf->regs[0]), [val] "d" (val)
#ifndef __FASTCALL__
		: "d2", "d3", "d4", "d5", "d6", "d7", "a2", "a3", "a4", "a5", "a6"
#else
		: "d3", "d4", "d5", "d6", "d7", "a2", "a3", "a4", "a5", "a6"
#endif
	);
};

#endif /* _SETJMP_H_ */
