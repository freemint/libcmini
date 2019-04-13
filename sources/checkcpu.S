| checkcpu.S -- MiNTLib.
|
| This file is part of the MiNTLib project, and may only be used
| modified and distributed under the terms of the MiNTLib project
| license, COPYMINT.  By continuing to use, modify, or distribute
| this file you indicate that you have read the license and
| understand and accept it fully.

| void _checkcpu(void);
|
| No need for __FASTCALL__ as long as we call _getcookie not _Getcookie.
|
| Check if the actual CPU is compatible with this binary.
| If it is compatible, the function returns silently.
| If it is not compatible, it displays an error message then exits with the
| error code -1. In that case, this function never returns.
|
| WARNING: This function is called from the startup code before knowing the
| actual CPU type. So it must be compatible with any processor (including
| 680x0 and ColdFire models). So it must be written carefully by using only
| instructions common to all of the supported processors. Especially, gas
| pseudo-jump instructions must be avoided (jbsr, jbra...), because they may
| be translated to unsupported jump instructions.

	.extern	_getcookie		| int Getcookie(long cookie, long *val);

	.globl	__checkcpu
__checkcpu:
#ifdef __M68020__
	subql	#4,sp			| Local variable for cookie value

	lea	sp@,a0
	movl	#0x5f435055,d0	| "_CPU"
	jsr	_getcookie

	movel	sp@+,d1			| Cookie value

	tstl	d0
	bnes	.bad020			| _CPU cookie not found

	cmpl	#20,d1
	bhss	.ok020			| CPU >= 68020

.bad020:
	pea	.err020			| Error message
	bra	.printexit

.err020:
	.ascii	"This program requires a 68020 or higher\r\n", \
		"processor and cannot be run on this\r\n", \
		"machine.\r\n\0"
	.even
.ok020:
#endif

#ifdef __mcoldfire__
	subql	#4,sp			| Local variable for cookie value

	lea	sp@,a0
	movl	#0x5f43465f,d0	| "_CF_"
	jsr	_getcookie

	movel	sp@+,d1			| Cookie value

	tstl	d0
	beqs	.okv4e			| _CF_ cookie found

	pea	.errv4e			| Error message
	bra	.printexit

.errv4e:
	.ascii	"This program requires a ColdFire V4e\r\n", \
		"processor and cannot be run on this\r\n", \
		"machine.\r\n\0"
	.even
.okv4e:
#endif

#ifdef __M68881__
	subql	#4,sp			| Local variable for cookie value

	lea	sp@,a0
	movl	#0x5f465055,d0	| "_FPU"
	jsr	_getcookie

	movel	sp@+,d1			| Cookie value

	tstl	d0
	bnes	.bad881			| _FPU cookie not found

	tstl	d1
	bnes	.ok881			| FPU present

.bad881:
	pea	.err881			| Error message
	bra	.printexit

.err881:
	.ascii	"This program requires a 68881 or higher\r\n", \
		"arithmetic coprocessor and cannot be run\r\n", \
		"on this machine.\r\n\0"
	.even
.ok881:
#endif
	rts				| All requirements passed

| Print a message then exit.
| The message must have been pushed on the stack.
.printexit:
	movew	#9,sp@-			| Cconws()
	trap	#1
	addql	#6,sp

	movew	#-1,sp@-		| Failure
	movew	#0x4c,sp@-		| Pterm()
	trap	#1
