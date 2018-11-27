| getsysvar.S -- MiNTLib.
|
| This file is part of the MiNTLib project, and may only be used
| modified and distributed under the terms of the MiNTLib project
| license, COPYMINT.  By continuing to use, modify, or distribute
| this file you indicate that you have read the license and
| understand and accept it fully.

| long get_sysvar(void *var);
|
| Read an OS variable.
|
| WARNING: This function is called from the startup code before knowing the
| actual CPU type. So it must be compatible with any processor (including
| 680x0 and ColdFire models). So it must be written carefully by using only
| instructions common to all of the supported processors. Especially, gas
| pseudo-jump instructions must be avoided (jbsr, jbra...), because they may
| be translated to unsupported jump instructions.

	.extern	___has_no_ssystem	| int __has_no_ssystem;

	.globl	_get_sysvar
_get_sysvar:
	lea	sp@(-16),sp
	movml	d2/d3/a2/a3,sp@		| ColdFire compatibility

#ifdef __FASTCALL__
	movel	a0,a3		| var
#else
	movel	sp@(16+4),a3		| var
#endif

	tstl	___has_no_ssystem
	beqs	.get_with_ssystem

	movel	#1,sp@-			| SUP_INQUIRE
	movew	#0x20,sp@-		| Super()
	trap	#1
	addql	#6,sp

	tstl	d0
	beqs	.get_usermode		| We are in user mode

	movel	a3@,d0			| Read the variable directly

.return:
	movml	sp@,d2/d3/a2/a3		| ColdFire compatibility
	lea	sp@(16),sp
	rts

.get_with_ssystem:
	clrl	sp@-
	movl	a3,sp@-			| var
	movew	#10,sp@-		| S_GETLVAL
	movew	#0x154,sp@-		| Ssystem()
	trap	#1
	lea	sp@(12),sp

	bras	.return

.get_usermode:
	clrl	sp@-			| SUP_SET
	movew	#0x20,sp@-		| Super()
	trap	#1			| Switch to supervisor mode
	addql	#6,sp

	movel	a3@,d3			| Get the variable value

	movel	d0,sp@-			| Old SSP
	movew	#0x20,sp@-		| Super()
	trap	#1			| Restore user mode
	addql	#6,sp

	movel	d3,d0

	bras	.return
