#ifndef _SETJMP_H_
#define _SETJMP_H_

typedef long jmp_buf[31];

extern int setjmp(jmp_buf buf);
extern void longjmp(jmp_buf, int);

#endif /* _SETJMP_H_ */
