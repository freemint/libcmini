#include <ctype.h>
#include <string.h>

char *strlwr(char *str)
{
    char *p = str;

    if (p)
    {
        while (*str)
        {
            if (isupper(*str))
                *str = tolower(*str);
            ++str;
        }
    }
    return p;
}
