#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../../data.h"
#include "../../utils/utils.h"

enum ROUTES 
path_to_route(char *path_buff)
{
	if (strstr(path_buff, "echo") != NULL) return ECHO;
	if (strcmp(path_buff, "user-agent") == 0) return USER_AGENT;
    if (strstr(path_buff, "files") != NULL) return FILES; 
	if (strcmp(path_buff, "/") == 0) return EMPTY;
	return INVALID_ROUTE;
}

char *
build_success_response(struct Response* resp)
{

    printf("building response...\n");

    char *SUCCESS_RESPONSE = malloc(1024);
    // char *SUCCESS_RESPONSE = ";

    char *content_buffer = malloc(512); 
    
    strcpy(SUCCESS_RESPONSE, "HTTP/1.1 200 OK\r\n");

    if (resp->content_type == NULL) {
        // required to add remianing CRLF headers
        strcat(SUCCESS_RESPONSE, "\r\n\r\n");
        return SUCCESS_RESPONSE;
    }

    strcat(SUCCESS_RESPONSE, resp->content_type);

    int trunc;
    if ( (trunc=snprintf(content_buffer, 512, "Content-Length: %d\r\n\r\n", resp->content_length)) < 0 ) {
        printf("Error converting string: %s \n", strerror(errno));
    }

    strcat(content_buffer, resp->content);
    strcat(SUCCESS_RESPONSE, content_buffer);

    return SUCCESS_RESPONSE;
}