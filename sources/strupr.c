#include <ctype.h>
#include <string.h>

char *strupr(char *str)
{
    char *p = str;

    if (p)
    {
        while (*str)
        {
            if (islower(*str))
                *str = toupper(*str);
            ++str;
        }
    }
    return p;
}
