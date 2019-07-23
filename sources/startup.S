|
| $Id: crt0.S,v 1.9 2009/06/15 18:44:08 alanh Exp $
|

|
| Initialization code; this is common to both 16 and 32 bit libraries,
| so be careful!
|
| WARNING: All the instructions before the call to _checkcpu() must be
| compatible with any processor (including 680x0 and ColdFire models).
| So it must be written carefully by using only instructions common to all
| of the supported processors. Especially, gas pseudo-jump instructions must
| be avoided (jbsr, jbra...), because they may be translated to unsupported
| jump instructions.

| No __FASTCALL__ modifications required.

|
| The following external symbols are declared with .globl rather than
| .extern in order to ensure they will be linked into the executable
| even if they are not used in this file.

	.globl	__app		| short, defined in crtinit.c
	.globl	__base		| BASEPAGE *, defined in globals.c
	.globl	__stksize	| long, defined by user or in stksiz.c
	.globl	___has_no_ssystem	| long, defined in globals.c

|
| externs to pull ident strings of all used libraries into the
| executable; if a library is not used, then the extern is
| satisfied by a dummy in the library

| nobody needs this
|	.globl	___Ident_gnulib
|	.globl	___Ident_socketlib
|	.globl	___Ident_gem

|
| Functions defined elsewhere.

	.extern	__checkcpu	| void _checkcpu(void);
	.extern	__acc_main	| void _acc_main(void);
	.extern	__crtinit	| void _crtinit(void);

|
| Assumption: basepage is passed in a0 for accessories; for programs
| a0 is always 0.

	.text
	.even
#ifdef __ELF__
	.section	.text.entry.mint,"ax",@progbits
#endif /* __ELF__ */
	.globl	__start
__start:
	subl	a6, a6		| clear a6 for debuggers
	cmpl	#0, a0		| test if acc or program
	beqs	__startprg	| if a program, go elsewhere
	tstl	a0@(36)		| also test parent basepage pointer
	bnes	__startprg	| for accs, it must be 0
	movel	a0, __base	| acc basepage is in A0
	lea	a0@(252), sp	| use the command line as a temporary stack
	jbsr	.early_init	| early initialization
	jsr	__checkcpu	| check for correct CPU or exit
	jmp	__acc_main	| function is in crtinit.c
|
| program startup code: doesn''t actually do much, other than push
| the basepage onto the stack and call _start1 in crtinit.c
|
__startprg:
	movel	sp@(4), a0	| get basepage
	movel	a0, __base	| save it
	movel	a0@(4), d0	| get _base->p_hitpa
	bclr	#0, d0		| round off
	movel	d0, sp		| set stack (temporarily)
	subl	#64, sp		| see note in mintlib/setstack.S
	jbsr	.early_init	| early initialization
	jsr	__checkcpu	| check for correct CPU or exit
	jmp	__crtinit	| in crtinit.c

|
| Initialize global variables required by early initialization code.
| Warning: The registers are not preserved
|
.early_init:
	clrl	sp@-
	clrl	sp@-
	movew	#-1,sp@-	| Check Ssystem() availability
	movew	#0x154,sp@-	| Ssystem()
	trap	#1
	lea	sp@(12),sp

	movel	d0,___has_no_ssystem

	rts

|
| interfaces for gprof: for crt0.o, does nothing, but for gcrt0.o branches
| to the appropriate subroutines
|
	.globl 	__monstartup
	.globl	__moncontrol
	.globl 	___mcleanup

#ifdef GCRT0
	.extern	_monstartup
	.extern	_moncontrol
	.extern	__mcleanup

__monstartup:
	jmp	_monstartup
__moncontrol:
	jmp	_moncontrol
___mcleanup:
	jmp	__mcleanup
#else
__monstartup:
__moncontrol:
___mcleanup:
	rts
#endif /* GCRT0 */
