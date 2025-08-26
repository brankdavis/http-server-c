#ifndef ROUTEHANDLERS_H
#define ROUTEHANDLERS_H

#define CONTENT_TYPE_TEXT "Content-Type: text/plain\r\n"
#define CONTENT_TYPE_OCTET "Content-Type: application/octet-stream\r\n"

#include "../data.h"

Response* handle_files_route(Request *request, int client, int server);
Response* handle_echo_route(Request *request, int client, int server);
Response* handle_user_agent_route(Request *request, int client, int server);
void handle_not_found(int client, int server, Response *response);


#endif