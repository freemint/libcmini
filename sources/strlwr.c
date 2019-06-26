#include <ctype.h>
#include <string.h>

char *strlwr(char *str)
{
    char *p = str;

    if (p)
    {
        do
        {
            if (isupper(*str)) {
                *str = tolower(*str);
			}
        } while (*str++ != '\0');
    }
    return p;
}
