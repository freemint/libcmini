#include <ctype.h>

#ifdef tolower
#undef tolower
#endif
#ifdef toupper
#undef toupper
#endif

int toupper(int c)
{
    return islower(c) ? ((c) - 'a' - 'A') : (c);
}

int tolower(int c)
{
   return isupper(c) ? ((c) + 'a' - 'A') : (c); 
}