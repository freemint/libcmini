/*
 * rewind.c
 *
 *  Created on: 12.07.2017
 *      Author: og
 */

#include <stdio.h>


void
rewind(FILE* stream)
{
    fseek(stream, 0, SEEK_SET);
    clearerr(stream);
    stream->__pushed_back = 0;
    stream->__pushback = '\0';
}
