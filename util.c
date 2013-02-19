#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "util.h"

int read_to_space(char *ret, char *buffer, int size, unsigned int *offset)
{
    int i = 0;
    int location = (int)*offset;
    if(location > 0)
        location++;
    else
        location = 0;
    while(location < size && !ispace(buffer[location]))
    {
        ret[i] = buffer[location];
        i++;
        location++;
    }
    ret[location] = '\0';
    *offset = location;
    return i;
}


void error(char const *err)
{
    perror(err);
}
