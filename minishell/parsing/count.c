#include <stdio.h>
#include <ctype.h>


int count_word(char *str)
{
    int i = 0;
    int count=0;
    while(isspace(str[i]))
        i++;
    while(str[i])
    {
        i++;
        if(isspace(str[i]) || str[i] == '\0')
            count++;
    }
    return(count);
}