#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../utils/utils.h"
#include "route_handlers.h"
#include "../data.h"
#include <sys/socket.h>



void
handle_not_found(int client, int server, struct Response *resp)
{
	char NOT_FOUND_RESPONSE[30] = "HTTP/1.1 404 Not Found\r\n\r\n";
    resp->code = NOT_FOUND;
    resp->content = NOT_FOUND_RESPONSE;
}

struct Response*
handle_files_route(char *req, char *resp, int client, int server)
{
    int bytes_read, trunc;
    char *file_path, *new_file_path;
    char read_buf[BUFSIZ], *buf_ptr;
    char filename_buf[BUFSIZ] = "/tmp/data/codecrafters.io/http-server-tester/";
    buf_ptr = read_buf;
    FILE *fptr;
    char *file_stream;
    char contentLengthBuf[50];
    struct Response *response = malloc(sizeof(struct Response));


	printf("handling files route: %s\n", req);
    

    // extract filename
    file_path = extract_req_path(req);
    strcat(filename_buf, file_path);

    // attempt to open file
    printf("attempting to open file %s\n", filename_buf);

    if ((fptr = fopen(filename_buf, "r")) == NULL) {
        printf("Error opening file\n", strerror(errno));
        handle_not_found(client, server, response);
        return response;
    }

    printf("file opened\n");

    // // if opened, read file
    while (fgets(read_buf, BUFSIZ-1, fptr) != NULL) {
        printf("%s\n", read_buf);
    }

    response->code = SUCCESS;
    response->content_type = "Content-Type: application/octet-stream\r\n";
    response->content = read_buf;
    response->content_length = strlen(read_buf);

    return response;
}

struct Response*
handle_user_agent_route(char *req, int client, int server)
{
	char* user_agent_str = strstr(req, USER_AGENT_HEADER);;
	char contentLengthBuf[50];
	int trunc;
    char SUCCESS_RESPONSETYPE_w_CONTENT_[100] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n";
    struct Response *response = malloc(sizeof(struct Response));

	printf("handle user agent route request: %s\n", req);
	if (user_agent_str == NULL) {
		printf("404");

	}

	user_agent_str += strlen(USER_AGENT_HEADER);

    response->code = SUCCESS;
    response->content_type = "Content-Type: text/plain\r\n";
    response->content = user_agent_str;
    response->content_length = strlen_noSpaces(user_agent_str);

    return response;

}

struct Response*
handle_echo_route(char *req_string_buff, int client, int server)
{
	req_string_buff = extract_req_path(req_string_buff);
	int content_length = strlen(req_string_buff);
	char contentLengthBuf[50];
	int trunc;
    char SUCCESS_RESPONSETYPE_w_CONTENT_[100] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n";
    struct Response *response = malloc(sizeof(struct Response));

    response->code = SUCCESS;
    response->content_type = "Content-Type: text/plain\r\n";
    response->content = req_string_buff;
    response->content_length = strlen(req_string_buff);

    return response;

}