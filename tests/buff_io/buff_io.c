/*
 * buff_io.c
 *
 *  Created on: 29.11.2018
 *      Author: mfro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fp;

    fp = fopen("test.txt", "w+");
    if (fp != NULL)
    {
        int i;

        for (i = 0; i < 1024; i++)
        {
            char str[1024];
            int res;

            sprintf(str, "this is line #%d\n", i);
            res = fwrite(str, 1, strlen(str), fp); 
            if (res != strlen(str))
            {
                fprintf(stderr, "error writing line %d\r\n", i);
                exit(1);
            }
        }
    }
    else
    {
        fprintf(stderr, "cannot open file\r\n");
        exit(1);
    }
    fclose(fp);
	
	Cconws("press a key to return to desktop\r\n");
	Cconin();
}
