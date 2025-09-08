#ifndef SERVER_HELPERS_H
#define SERVER_HELPERS_H

#include "../../data.h"

ROUTES path_to_route(char *path_buff);
REQUEST_METHODS extract_request_method(char *req);
Request* construct_request(char* request_string);
char *build_success_response(Response* resp, size_t *body_len);
void free_request(Request *req);
void check_compression(Response *resp, Request *req);

#endif