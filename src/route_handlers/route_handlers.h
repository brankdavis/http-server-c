#ifndef ROUTEHANDLERS_H
#define ROUTEHANDLERS_H

struct Response* handle_files_route(char *req, char *resp, int client, int server);
struct Response* handle_echo_route(char *req_string_buff, int client, int server);
struct Response* handle_user_agent_route(char *req, int client, int server);
void handle_not_found(int client, int server, struct Response *response);


#endif