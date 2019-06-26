#include <ctype.h>
#include <string.h>

char *strupr(char *str)
{
    char *p = str;

    if (p)
    {
        do
        {
            if (islower(*str)) {
                *str = toupper(*str);
			}
        } while (*str++ != '\0');
    }
    return p;
}
