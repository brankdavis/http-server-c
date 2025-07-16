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

void*
server(void *arg)
{
		struct thread_args *args = (struct thread_args*)arg;
		struct Response *response = malloc(sizeof(struct Response));
		printf("Client connected\n");
		printf("client %d\n", args->client_fd);
		printf("server %d\n", args->server_fd);
		

		char *buff_ptr, resp_ptr;
		char *resp_buf, SUCCESS_RESPONSETYPE_w_CONTENT_[100] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n";
		char extracted_url_path_buff[100] = "";
		int extracted_path_buff_index = 0;
		int content_length = 0;
		int res;
		char buf[BUFSIZ];
		int bytes_read;
		char SUCCESS_RESPONSE[100] = "HTTP/1.1 200 OK\r\n\r\n";

		if ((bytes_read = read(args->client_fd, buf, BUFSIZ-1)) < 0 ) {
			printf("Socket Read Error: %s \n", strerror(errno));
		}

		buff_ptr = buf;

		//sl.protocol = GET; // Future: need more robust method to get protocol
		for(; *buff_ptr != '/'; buff_ptr++){
			
			
		}

		printf("request: %c\n", *buff_ptr);


		buff_ptr++;

		if ( *buff_ptr == ' ' ) 
		{
			printf("EMPTY REAQUESTSTRING %c\n", *buff_ptr);
            response->code = SUCCESS;
		} 
		else 
		{

			// extract url path
			for(; *buff_ptr != ' '; buff_ptr++){
				extracted_url_path_buff[extracted_path_buff_index++] = *buff_ptr;
			}

			enum ROUTES extracted_route = path_to_route(extracted_url_path_buff);
			switch(extracted_route) {
				case ECHO: 
					response = handle_echo_route(extracted_url_path_buff, args->client_fd, args->server_fd);
					break;
				
				case USER_AGENT:
					response = handle_user_agent_route(buff_ptr, args->client_fd, args->server_fd);
					break;
				
				case FILES:
					response = handle_files_route(extracted_url_path_buff, resp_buf, args->client_fd, args->server_fd);
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

		// handle response
		if (response->code == SUCCESS) {
			
			// build response
			resp_buf = build_success_response(response);
            printf(" ### DEBUG ### response buffer: %s\n", resp_buf);
            send(args->client_fd, resp_buf, strlen(resp_buf), 0);
		}
        else {

            // send 404 response
            send(args->client_fd, response->content, strlen(response->content), 0);

        }

}