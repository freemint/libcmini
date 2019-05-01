#include <stdio.h>
#include <string.h>

void testcmp(const char* s1, const char* s2)
{
    int cmp = strcmp(s1, s2);
    if (cmp>0) {
        printf("'%s' > '%s'\n", s1, s2);
    } else if (cmp<0) {
        printf("'%s' < '%s'\n", s1, s2);
    } else {
        printf("'%s' = '%s'\n", s1, s2);
    }
}

int main(void)
{
    testcmp("abc", "def");
    testcmp("def", "def");
    testcmp("def", "abc");
    
    testcmp("abc", "");
    testcmp("abc", "ab");
    testcmp("", "abc");
    testcmp("ab", "abc");

    return 0;
}
