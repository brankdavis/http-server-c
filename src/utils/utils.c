#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "utils.h"
#include "../data.h"



//#define DEBUG

#ifdef DEBUG
    #include <stdio.h>
#endif


int strlen_noSpaces(char *str) 
{
    int count = 0;
    while (*str != '\0')
    {
        if (!isspace(*str)) {
            #ifdef DEBUG
            printf("is alpha: %c\n", *str);
            #endif
            count++;
        };

        str++;
    }

    return count;

}

int strlen_noEcho(char *str) 
{
    str = strstr(str, "/");
    return strlen(str) - 1;

}

char* extract_req_path(char* str) 
{
    str = strstr(str, "/");
    return ++str;
}