#ifndef SERVER_HELPERS_H
#define SERVER_HELPERS_H

#include "../../data.h"

enum ROUTES path_to_route(char *path_buff);
enum REQUEST_METHODS extract_request_method(char *req);
Request* construct_request(char* request_string);
char *build_success_response(Response* resp);
void free_request(Request *req);

#endif