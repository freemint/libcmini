#include <ctype.h>

#ifdef toupper
#undef toupper
#endif

int toupper(int c)
{
    return islower(c) ? ((c) - 'a' - 'A') : (c);
}
