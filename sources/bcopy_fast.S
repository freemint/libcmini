#ifdef __FASTCALL__

|	new version of bcopy, memcpy and memmove
|	handles overlap, odd/even alignment
|	uses movem to copy 256 bytes blocks faster.
|	Alexander Lehmann	alexlehm@iti.informatik.th-darmstadt.de
|	sortof inspired by jrbs bcopy

	.text
	.even
	.globl ___bcopy
	.globl __bcopy
	.globl _bcopy
	.globl _memcpy
	.globl _memmove

|	void bcopy( const void *src, void *dest, size_t length );
|	void _bcopy( const void *src, void *dest, unsigned long length );
|	return value not used (returns src)
|	functions are aliased (except for HSC -- sb)

_bcopy:
___bcopy:
__bcopy:
	exg		a0,a1
	jra	common		| the rest is samea as memcpy

|	void *memcpy( void *dest, const void *src, size_t len );
|	void *memmove( void *dest, const void *src, size_t len );
|	returns dest
|	functions are aliased

_memcpy:
_memmove:

common:
	tstl	d0	
	jeq	exit		| length==0? (size_t)

				| a1 src, a0 dest, d0.l length
	movel	a0,sp@-

	| overlay ?
	cmpl	a1,a0
	jgt	top_down

#ifdef __mcoldfire__
	movl	a1,d1		| test for alignment
	movl	a0,d2
	eorl	d2,d1
#else
	movw	a1,d1		| test for alignment
	movw	a0,d2
	eorw	d2,d1
#endif
	btst	#0,d1		| one odd one even ?
	jne	slow_copy
	btst	#0,d2		| both even ?
	jeq	both_even
	movb	a1@+,a0@+	| copy one byte, now we are both even
	subql	#1,d0
both_even:
	movq	#0,d1		| save length less 256
	movb	d0,d1
	lsrl	#8,d0		| number of 256 bytes blocks
	jeq	less256
#ifdef __mcoldfire__
	lea	sp@(-40),sp
	movml	d1/d3-d7/a2/a3/a5/a6,sp@	| d2 is already saved
					| exclude a4 because of -mbaserel
copy256:
	movml	a1@,d1-d7/a2/a3/a5/a6	| copy 5*44+36=256 bytes
	movml	d1-d7/a2/a3/a5/a6,a0@
	movml	a1@(44),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(44)
	movml	a1@(88),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(88)
	movml	a1@(132),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(132)
	movml	a1@(176),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(176)
	movml	a1@(220),d1-d7/a2-a3
	movml	d1-d7/a2-a3,a0@(220)
	lea	a1@(256),a1
#else
	movml	d1/d3-d7/a2/a3/a5/a6,sp@-	| d2 is already saved
					| exclude a4 because of -mbaserel
copy256:
	movml	a1@+,d1-d7/a2/a3/a5/a6	| copy 5*44+36=256 bytes
	movml	d1-d7/a2/a3/a5/a6,a0@
	movml	a1@+,d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(44)
	movml	a1@+,d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(88)
	movml	a1@+,d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(132)
	movml	a1@+,d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(176)
	movml	a1@+,d1-d7/a2-a3
	movml	d1-d7/a2-a3,a0@(220)
#endif
	lea	a0@(256),a0		| increment dest, src is already
	subql	#1,d0
	jne	copy256 		| next, please
#ifdef __mcoldfire__
	movml	sp@,d1/d3-d7/a2/a3/a5/a6
	lea	sp@(40),sp
less256:			| copy 16 bytes blocks
	movl	d1,d0
	lsrl	#2,d0		| number of 4 bytes blocks
	jeq	less4		| less that 4 bytes left
	movl	d0,d2
	negl	d2
	andil	#3,d2		| d2 = number of bytes below 16 (-n)&3
	subql	#1,d0
	lsrl	#2,d0		| number of 16 bytes blocks minus 1, if d2==0
	addl	d2,d2		| offset in code (movl two bytes)
	jmp	pc@(2,d2:l)	| jmp into loop
#else
	movml	sp@+,d1/d3-d7/a2/a3/a5/a6
less256:			| copy 16 bytes blocks
	movw	d1,d0
	lsrw	#2,d0		| number of 4 bytes blocks
	jeq	less4		| less that 4 bytes left
	movw	d0,d2
	negw	d2
	andiw	#3,d2		| d2 = number of bytes below 16 (-n)&3
	subqw	#1,d0
	lsrw	#2,d0		| number of 16 bytes blocks minus 1, if d2==0
	addw	d2,d2		| offset in code (movl two bytes)
	jmp	pc@(2,d2:w)	| jmp into loop
#endif
copy16:
	movl	a1@+,a0@+
	movl	a1@+,a0@+
	movl	a1@+,a0@+
	movl	a1@+,a0@+
#ifdef __mcoldfire__
	subql	#1,d0
	bpl	copy16
#else
	dbra	d0,copy16
#endif
less4:
	btst	#1,d1
	jeq	less2
	movw	a1@+,a0@+
less2:
	btst	#0,d1
	jeq	none
	movb	a1@,a0@
none:
exit_d2:
	movl	sp@+,a0
exit:
	rts

slow_copy:			| byte by bytes copy
#ifdef __mcoldfire__
	movl	d0,d1
	negl	d1
	andil	#7,d1		| d1 = number of bytes blow 8 (-n)&7
	addql	#7,d0
	lsrl	#3,d0		| number of 8 bytes block plus 1, if d1!=0
	addl	d1,d1		| offset in code (movb two bytes)
	jmp	pc@(2,d1:l)	| jump into loop
#else
	movw	d0,d1
	negw	d1
	andiw	#7,d1		| d1 = number of bytes blow 8 (-n)&7
	addql	#7,d0
	lsrl	#3,d0		| number of 8 bytes block plus 1, if d1!=0
	addw	d1,d1		| offset in code (movb two bytes)
	jmp	pc@(2,d1:w)	| jump into loop
#endif
scopy:
	movb	a1@+,a0@+
	movb	a1@+,a0@+
	movb	a1@+,a0@+
	movb	a1@+,a0@+
	movb	a1@+,a0@+
	movb	a1@+,a0@+
	movb	a1@+,a0@+
	movb	a1@+,a0@+
	subql	#1,d0
	jne	scopy
	jra	exit_d2

top_down:
	addl	d0,a1		| a1 byte after end of src
	addl	d0,a0		| a0 byte after end of dest

#ifdef __mcoldfire__
	movl	a1,d1		| exact the same as above, only with predec
	movl	a0,d2
	eorl	d2,d1
#else
	movw	a1,d1		| exact the same as above, only with predec
	movw	a0,d2
	eorw	d2,d1
#endif
	btst	#0,d1
	jne	slow_copy_d

	btst	#0,d2
	jeq	both_even_d
	movb	a1@-,a0@-
	subql	#1,d0
both_even_d:
	movq	#0,d1
	movb	d0,d1
	lsrl	#8,d0
	jeq	less256_d
#ifdef __mcoldfire__
	lea	sp@(-40),sp
	movml	d1/d3-d7/a2/a3/a5/a6,sp@
copy256_d:
	movml	a1@(-44),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(-44)
	movml	a1@(-88),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(-88)
	movml	a1@(-132),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(-132)
	movml	a1@(-176),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(-176)
	movml	a1@(-220),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@(-220)
	movml	a1@(-256),d1-d7/a2-a3
	movml	d1-d7/a2-a3,a0@(-256)
	lea	a0@(-256),a0
#else
	movml	d1/d3-d7/a2/a3/a5/a6,sp@-
copy256_d:
	movml	a1@(-44),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@-
	movml	a1@(-88),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@-
	movml	a1@(-132),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@-
	movml	a1@(-176),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@-
	movml	a1@(-220),d1-d7/a2/a3/a5/a6
	movml	d1-d7/a2/a3/a5/a6,a0@-
	movml	a1@(-256),d1-d7/a2-a3
	movml	d1-d7/a2-a3,a0@-
#endif
	lea	a1@(-256),a1
	subql	#1,d0
	jne	copy256_d
#ifdef __mcoldfire__
	movml	sp@,d1/d3-d7/a2/a3/a5/a6
	lea	sp@(40),sp
less256_d:
	movl	d1,d0
	lsrl	#2,d0
	jeq	less4_d
	movl	d0,d2
	negl	d2
	andil	#3,d2
	subql	#1,d0
	lsrl	#2,d0
	addl	d2,d2
	jmp	pc@(2,d2:l)
#else
	movml	sp@+,d1/d3-d7/a2/a3/a5/a6
less256_d:
	movw	d1,d0
	lsrw	#2,d0
	jeq	less4_d
	movw	d0,d2
	negw	d2
	andiw	#3,d2
	subqw	#1,d0
	lsrw	#2,d0
	addw	d2,d2
	jmp	pc@(2,d2:w)
#endif
copy16_d:
	movl	a1@-,a0@-
	movl	a1@-,a0@-
	movl	a1@-,a0@-
	movl	a1@-,a0@-
#ifdef __mcoldfire__
	subql	#1,d0
	bpl	copy16_d
#else
	dbra	d0,copy16_d
#endif
less4_d:
	btst	#1,d1
	jeq	less2_d
	movw	a1@-,a0@-
less2_d:
	btst	#0,d1
	jeq	exit_d2
	movb	a1@-,a0@-
	jra	exit_d2
slow_copy_d:
#ifdef __mcoldfire__
	movl	d0,d1
	negl	d1
	andil	#7,d1
	addql	#7,d0
	lsrl	#3,d0
	addl	d1,d1
	jmp	pc@(2,d1:l)
#else
	movw	d0,d1
	negw	d1
	andiw	#7,d1
	addql	#7,d0
	lsrl	#3,d0
	addw	d1,d1
	jmp	pc@(2,d1:w)
#endif
scopy_d:
	movb	a1@-,a0@-
	movb	a1@-,a0@-
	movb	a1@-,a0@-
	movb	a1@-,a0@-
	movb	a1@-,a0@-
	movb	a1@-,a0@-
	movb	a1@-,a0@-
	movb	a1@-,a0@-
	subql	#1,d0
	jne	scopy_d
	jra	exit_d2

#endif /* __FASTCALL__ */