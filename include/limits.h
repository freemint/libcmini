#ifndef _LIMITS_H_
#define _LIMITS_H_


#define CHAR_BIT	8
#define SCHAR_MAX	0x7f
#define SCHAR_MIN	(-SCHAR_MAX - 1)
#define UCHAR_MAX	0xffU
#ifdef __CHAR_UNSIGNED__
# define CHAR_MAX	UCHAR_MAX
# define CHAR_MIN	0
#else
# define CHAR_MAX	SCHAR_MAX
# define CHAR_MIN	SCHAR_MIN
#endif

#define SHRT_MAX	0x7fff
#define USHRT_MAX	0xffffU
#define SHRT_MIN	(-SHRT_MAX - 1)

#define LONG_MAX	0x7fffffffL
#define ULONG_MAX	0xffffffffUL
#define LONG_MIN	(-LONG_MAX - 1L)

#ifndef __MSHORT__
# define INT_MAX	SHRT_MAX
# define UINT_MAX	USHRT_MAX
#else
# define INT_MAX	LONG_MAX
# define UINT_MAX	ULONG_MAX
#endif
#define INT_MIN	(-INT_MAX - 1)

#endif /* _LIMITS_H_ */