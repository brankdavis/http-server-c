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
handle_not_found(int client, int server, Response *resp)
{
	char NOT_FOUND_RESPONSE[30] = "HTTP/1.1 404 Not Found\r\n\r\n";
    resp->code = NOT_FOUND;
    resp->content = NOT_FOUND_RESPONSE;
}

static inline COMPRESSION_SCHEMES
get_compression_scheme(Request *req)
{
    if (req && req->headers && req->headers->accept_encoding) {
        if (strcmp(req->headers->accept_encoding, GZIP) == 0) {
            return gzip;
        }
    }
    return UNSUPPORTED_SCHEME;
}

static Response* 
handle_get_files(char *filename_buf, int client, int server)
{
    Response *response = malloc(sizeof(Response));
    char read_buf[BUFSIZ];
    FILE *fptr;

    printf("handle GET files\n");

    // attempt to open file
    printf("attempting to open file %s\n", filename_buf);

    if ((fptr = fopen(filename_buf, "r")) == NULL) {
        printf("Error opening file %s\n", strerror(errno));
        handle_not_found(client, server, response);
        return response;
    }

    printf("file opened\n");

    // if opened, read file
    while (fgets(read_buf, BUFSIZ-1, fptr) != NULL) {
        printf("%s\n", read_buf);
    }

    response->code = SUCCESS;
    response->content_type = CONTENT_TYPE_OCTET;
    response->content = read_buf;
    response->content_length = strlen(read_buf);
    return response;

};

static Response*
handle_post_files(char *filename_buf, Request *req)
{

    printf("handle POST files at this location: %s\n", filename_buf);
    int fd;
    ssize_t write_size;
    Response *response = malloc(sizeof(Response));
    FILE *fptr;

    // create new file
    if ((fptr = fopen(filename_buf, "w")) == NULL) {
        printf("Error creating file %s\n", strerror(errno));
        response->code = SERVER_ERROR;
        return response;
    }


    // write to file
    if ((write_size = fwrite(req->request_body, 1, req->headers->content_length, fptr)) < 1) {
        printf("Error writing to file %s\n", strerror(errno));
        response->code = SERVER_ERROR;
        return response;
    }

    response->code = CREATED;

    fflush(fptr);
    fclose(fptr);

    return response;
};

Response*
handle_files_route(Request *request, int client, int server)
{
    int bytes_read, trunc;
    char *file_path, *new_file_path;
    char read_buf[BUFSIZ], *buf_ptr;
    char filename_buf[BUFSIZ] = "/tmp/data/codecrafters.io/http-server-tester/", *cc_file_ptr;
    buf_ptr = read_buf;
    cc_file_ptr = filename_buf;
    FILE *fptr;
    char *file_stream;
    char contentLengthBuf[50];
    Response *response = malloc(sizeof(Response));

    // extract filename
    file_path = extract_req_path(request->url);
    strcat(filename_buf, file_path);


    switch (request->http_method)
    {
    case GET:
        response = handle_get_files(cc_file_ptr, client, server);
        break;
    
    case POST:
        response = handle_post_files(cc_file_ptr, request);
        break;
    
    default:
        printf("### IDK WHAT THIS IS....ABORT!! ###");
        break;
    }

    return response;
}

Response*
handle_user_agent_route(Request *request,  int client, int server)
{
	char * user_agent_str = request->headers->user_agent;
    Response *response = malloc(sizeof(Response));

	printf("handle user agent route request: %s\n", request->headers->user_agent);
	if (user_agent_str == NULL) {
		printf("404");

	}

    response->code = SUCCESS;
    response->content_type = CONTENT_TYPE_TEXT;
    response->content = user_agent_str;
    response->content_length = strlen_noSpaces(user_agent_str);

    return response;

}

Response*
handle_echo_route(Request *request, int client, int server)
{
	String req_string = extract_req_path(request->url);
	int content_length = strlen(req_string);
    Response *response = malloc(sizeof(Response));

    COMPRESSION_SCHEMES scheme = get_compression_scheme(request);

    switch (scheme)
    {
    case gzip:
        response->content_encoding = GZIP;
        break;
    
    default:
        break;
    }

    response->code = SUCCESS;
    response->content_type = CONTENT_TYPE_TEXT;
    response->content = req_string;
    response->content_length = content_length;

    return response;

}