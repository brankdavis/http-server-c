#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "helpers/helpers.h"
#include "../data.h"
#include "../route_handlers/route_handlers.h"


/*
	HTTP Server:

	step 1. Construct Request
	step 2. Process Request
	step 3. Construct Response
	step 4. Send to client
*/
void*
server(void *arg)
{
		Server_args *args = (Server_args*)arg;
		Response *response = malloc(sizeof(Response));
		Request *request = malloc(sizeof(Request));

		char *resp_buf;
		char buf[BUFSIZ];
		int bytes_read;
		char SUCCESS_RESPONSE[100] = "HTTP/1.1 200 OK\r\n\r\n";

		if ((bytes_read = read(args->client_fd, buf, BUFSIZ-1)) < 0 ) {
			printf("Socket Read Error: %s \n", strerror(errno));
		}

		// step 1. Construct request
		request = construct_request(buf);
		
		// step 2. Process Request
		if (strlen(request->url) == 0) 
		{
			printf("EMPTY REQUESTSTRING \n");
            response->code = SUCCESS;
			response->content_length = 0;
		} 
		else 
		{
			enum ROUTES extracted_route = path_to_route(request->url);

			switch(extracted_route) {
				case ECHO: 
					response = handle_echo_route(request, args->client_fd, args->server_fd);
					break;
				
				case USER_AGENT:
					response = handle_user_agent_route(request, args->client_fd, args->server_fd);
					break;
				
				case FILES:
					response = handle_files_route(request, args->client_fd, args->server_fd);
					break;

				case EMPTY:
					printf("handle empty route\n");
					handle_not_found(args->client_fd, args->server_fd, response);
					break;

				default:
					printf("route not supported\n");
					response->code = NOT_FOUND;
					response->content = "HTTP/1.1 404 Not Found\r\n\r\n";
			}

		}

		printf("return response: in server: %d\n", response->code);

		// step 3. Construct Response
		if (response->code == SUCCESS) {
			
			resp_buf = build_success_response(response);
            printf(" ### DEBUG ### response buffer: %s\n", resp_buf);
            send(args->client_fd, resp_buf, strlen(resp_buf), 0);
		}
		else if (response->code == CREATED) {
			printf("response CREATED\n");
    		char *SUCCESS_RESPONSE = "HTTP/1.1 201 Created\r\n\r\n";
			send(args->client_fd, SUCCESS_RESPONSE, strlen(SUCCESS_RESPONSE), 0);
		}
		else if (response->code == SERVER_ERROR) {
			printf("server error;\n");
			close(args->server_fd);
		}
        else {
            // send 4xx response
            send(args->client_fd, response->content, strlen(response->content), 0);
        }

		free_request(request);

}