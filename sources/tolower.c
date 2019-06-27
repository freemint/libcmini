#include <ctype.h>

#ifdef tolower
#undef tolower
#endif

int tolower(int c)
{
   return isupper(c) ? ((c) + 'a' - 'A') : (c);
}
