#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../../data.h"
#include "../../utils/utils.h"

ROUTES 
path_to_route(char *path_buff)
{
	if (strstr(path_buff, "echo") != NULL) return ECHO;
	if (strcmp(path_buff, "user-agent") == 0) return USER_AGENT;
    if (strstr(path_buff, "files") != NULL) return FILES; 
	if (strcmp(path_buff, "/") == 0) return EMPTY;
	return INVALID_ROUTE;
}

char *trim(char *s) {
    // remove trailing whitespace
    char *end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\r' || *end == '\n')) {
        *end = '\0';
        end--;
    }
    return s;
}

REQUEST_METHODS 
extract_request_method(char *req)
{
    printf("extract_request_method input: %s\n", req);
    trim(req);

    if (strcmp(req, "GET") == 0) return GET;
    if (strcmp(req, "POST") == 0) return POST;
    return UNSUPPORTED_METHOD;
}

Request*
construct_request(char* request_string) 
{
    Request *request = malloc(sizeof(Request));
    Headers *headers = malloc(sizeof(Headers));

    char *ptr = request_string;
    char http_method_buf[10] = "";
    char url_buff[BUFSIZ];
    char http_version_buff[10];
    char host_buff[BUFSIZ];
    char user_agent_buff[BUFSIZ];
    char accept_buff[BUFSIZ];
    char accept_encoding_buff[BUFSIZ];
    char content_type_bUff[BUFSIZ];
    char content_len_buff[BUFSIZ];


    // ---- request method ----
    int index = 0;
    for(; *ptr != '/'; ptr++, index++){
        http_method_buf[index] = *ptr;
    }
    request->http_method = extract_request_method(http_method_buf);
    ptr++; // skip space

    // ---- url ----
    index = 0;
    for(; *ptr !=  ' '; ptr++, index++) {
        url_buff[index] = *ptr;
    }
    url_buff[index] = '\0';
    request->url = strdup(url_buff);
    ptr++;

    // ---- http version ----
    index = 0;
    char *http = strstr(ptr, "HTTP");
    for(; !(http[0] == '\r' && http[1] == '\n'); http++, index++) {
        http_version_buff[index] = *http;
    }
    http_version_buff[index] = '\0';
    request->version = strdup(http_version_buff);

    // move ptr to after the request line CRLF
    ptr = strstr(ptr, "\r\n") + 2;


    /* Headers */

    // ---- Host ----
    index = 0;
    char *host = strstr(ptr, "Host:");
    if (host) {
        host += strlen("Host:");
        while (*host == ' ') host++; // skip whitespace
        for(; !(host[0] == '\r' && host[1] == '\n'); host++, index++) {
            host_buff[index] = *host;
        }
        host_buff[index] = '\0';
        headers->host = strdup(host_buff);
        
    } else {
        headers->host = NULL;
    }
    
    // ---- User-Agent ----
    index = 0;
    char *ua = strstr(ptr, "User-Agent:");
    if (ua) {
        ua += strlen("User-Agent:");
        while (*ua == ' ') ua++;
        for(; !(ua[0] == '\r' && ua[1] == '\n'); ua++, index++) {
            user_agent_buff[index] = *ua;
        }
        user_agent_buff[index] = '\0';
        headers->user_agent = strdup(user_agent_buff);
    } else {
        headers->user_agent = NULL;
    }

    // ---- Accept ----
    index = 0;
    char *accept = (strstr(ptr, "Accept:"));
    if (accept) {
        accept += strlen("Accept:");
        while(*accept == ' ') accept++;
        for(; !(accept[0] == '\r' && accept[1] == '\n'); accept++, index++) {
            accept_buff[index] = *accept;
        }
        accept_buff[index] = '\0';
        headers->accept = strdup(accept_buff);
    } else {
        headers->accept = NULL;
    }

    // ---- Accept-Encoding ----
    index = 0;
    char * accept_encoding = (strstr(ptr, "Accept-Encoding:"));
    if (accept_encoding) {
        accept_encoding += strlen("Accept-Encoding:");
        while(*accept_encoding == ' ') accept_encoding++;
        for(; !(accept_encoding[0] == '\r' && accept_encoding[1] == '\n'); accept_encoding++, index++) {
            accept_encoding_buff[index] = *accept_encoding;
        }
        accept_encoding_buff[index] = '\0';
        headers->accept_encoding = strdup(accept_encoding_buff);
    } else {
        headers->accept_encoding = NULL;
    }

    // -- Content-Type ----
    index = 0;
    char *ct_type = (strstr(ptr, "Content-Type:"));
    if (ct_type) {
        ct_type += strlen("Content-Type:");
        while(*ct_type == ' ') ++ct_type;
        for(; !(ct_type[0] == '\r' && ct_type[1] == '\n'); ct_type++, index++) {
        content_type_bUff[index] = *ct_type;
        }
        content_type_bUff[index] = '\0';
        headers->content_type = strdup(content_type_bUff);
    } else {
        headers->content_type = NULL;
    }

    // ---- Content-Length ----
    index = 0;
    char *ct_len = strstr(ptr, "Content-Length:");
    if (ct_len) {
        ct_len += strlen("Content-Length:");
        while (*ct_len == ' ') ++ct_len;
        for(; !(ct_len[0] == '\r' && ct_len[1] == '\n'); ct_len++, index++) {
            content_len_buff[index] = *ct_len;
        }
        content_len_buff[index] = '\0';
        headers->content_length = atoi(content_len_buff);
    } else {
        headers->content_length = 0;
    }

    request->headers = headers;

    // ---- Body ----
    char *body = strstr(ptr, "\r\n\r\n"); 
    if (body) {
        body += 4; // skip CRLFCRLF
        request->request_body = strdup(body);
    } else {
        request->request_body = NULL;
    }

    return request;

}

// --- Cleanup ---
void 
free_request(Request *req) {
    if (!req) return;
    free(req->url);
    free(req->version);
    if (req->request_body) free(req->request_body);

    if (req->headers) {
        free(req->headers->host);
        free(req->headers->user_agent);
        free(req->headers->accept);
        free(req->headers->content_type);
        free(req->headers);
    }
    free(req);
}

char *
build_success_response(Response* resp)
{
    printf("building response...\n");

    char *SUCCESS_RESPONSE = malloc(BUFSIZ);
    char *content_buffer = malloc(512);
    char *content_enc_buffer = malloc(512);
    
    strcpy(SUCCESS_RESPONSE, "HTTP/1.1 200 OK\r\n");

    printf("resp content: %s\n", resp->content);

    if (resp->content_length < 1) {
        // required to add remianing CRLF headers
        strcat(SUCCESS_RESPONSE, "\r\n\r\n");
        return SUCCESS_RESPONSE;

    } else {
        printf("Building content char *: %s\n", resp->content);
        strcat(SUCCESS_RESPONSE, resp->content_type);

        int offset;

        if (resp->content_encoding != NULL) {
            if ( (offset =snprintf(content_enc_buffer, 512, "Content-Encoding: %s\r\n\r\n", resp->content_encoding)) < 0 ) {
                printf("Error interpolating Content Encoding: %s \n", strerror(errno));
            }

            strcat(SUCCESS_RESPONSE, content_enc_buffer);
        }

        offset = 0;
        if ( (offset=snprintf(content_buffer, 512, "Content-Length: %d\r\n\r\n", resp->content_length)) < 0 ) {
                printf("Error interpolating Content Length: %s \n", strerror(errno));
        }
        strcat(content_buffer, resp->content);
        strcat(SUCCESS_RESPONSE, content_buffer);
    }

    return SUCCESS_RESPONSE;
}