#ifndef _MATH_PRIVATE_H_
#error "Never use <math_ldbl.h> directly; include <math_private.h> instead."
#endif

#ifdef __mcoldfire__
#  define NO_LONG_DOUBLE 1
#endif

/* A union which permits us to convert between a long double and
   four 32 bit ints or two 64 bit ints.  */

#ifndef NO_LONG_DOUBLE

#if __FLOAT_WORD_ORDER == BIG_ENDIAN

typedef union
{
  long double value;
  struct
  {
    __uint64_t msw;
    __uint64_t lsw;
  } parts64;
  struct
  {
    __uint32_t w0, w1, w2, w3;
  } parts32;
} ieee854_long_double_shape_type;

#endif

#if __FLOAT_WORD_ORDER == LITTLE_ENDIAN

typedef union
{
  long double value;
  struct
  {
    __uint64_t lsw;
    __uint64_t msw;
  } parts64;
  struct
  {
    __uint32_t w3, w2, w1, w0;
  } parts32;
} ieee854_long_double_shape_type;

#endif

/* Get two 64 bit ints from a long double.  */

#define GET_LDOUBLE_WORDS64(ix0,ix1,d)				\
do {								\
  ieee854_long_double_shape_type qw_u;				\
  qw_u.value = (d);						\
  (ix0) = qw_u.parts64.msw;					\
  (ix1) = qw_u.parts64.lsw;					\
} while (0)

/* Set a long double from two 64 bit ints.  */

#define SET_LDOUBLE_WORDS64(d,ix0,ix1)				\
do {								\
  ieee854_long_double_shape_type qw_u;				\
  qw_u.parts64.msw = (ix0);					\
  qw_u.parts64.lsw = (ix1);					\
  (d) = qw_u.value;						\
} while (0)

/* Get the more significant 64 bits of a long double mantissa.  */

#define GET_LDOUBLE_MSW64(v,d)					\
do {								\
  ieee854_long_double_shape_type sh_u;				\
  sh_u.value = (d);						\
  (v) = sh_u.parts64.msw;					\
} while (0)

/* Set the more significant 64 bits of a long double mantissa from an int.  */

#define SET_LDOUBLE_MSW64(d,v)					\
do {								\
  ieee854_long_double_shape_type sh_u;				\
  sh_u.value = (d);						\
  sh_u.parts64.msw = (v);					\
  (d) = sh_u.value;						\
} while (0)

/* Get the least significant 64 bits of a long double mantissa.  */

#define GET_LDOUBLE_LSW64(v,d)					\
do {								\
  ieee854_long_double_shape_type sh_u;				\
  sh_u.value = (d);						\
  (v) = sh_u.parts64.lsw;					\
} while (0)

#endif /* __mcoldfire__ */
