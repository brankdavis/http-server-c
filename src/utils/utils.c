#include <string.h>
#include "utils.h"
#include <ctype.h>
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

enum Route 
path_to_route(char *path_buff)
{
	if (strstr(path_buff, "echo") != NULL) return ECHO;
	if (strcmp(path_buff, "user-agent") == 0) return USER_AGENT;
	if (strcmp(path_buff, "/") == 0) return EMPTY;
	return INVALID_ROUTE;
}