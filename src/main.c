#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define DEBUG_EXTRACTED_URL
#define ECHO_ROUTE_LENGTH 4

char SUCCESS_RESPONSE[100] = "HTTP/1.1 200 OK\r\n\r\n";
char SUCCESS_RESPONSETYPE_w_CONTENT_[100] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n";

enum Route {
	INVALID_ROUTE = -1,
	EMPTY,
	ECHO,
	USER_AGENT
};

void
move_to_start_of_path(char *buff) 
{
	printf("BUFF STARTS WITH %c\n", buff[0]);
	for(; *buff != '/'; buff++){
		#ifdef DEBUG_EXTRACTED_URL
			//printf("BUFF STARTS WITH %c", buff[0]);
		#endif
	}
	printf("BUFF AFTER LOOP %c\n", *buff);
}

void
handle_not_found(int client, int server)
{
	char NOT_FOUND_RESPONSE[30] = "HTTP/1.1 404 Not Found\r\n\r\n";
	send(client, NOT_FOUND_RESPONSE, strlen(NOT_FOUND_RESPONSE), 0);
	close(server);
}

void
handle_route(char *buff_ptr,  int route, int client, int server)
{
	printf("handling route: %s", buff_ptr);

	if (route == ECHO) {
		printf("handle echo route...\n");

		char req_str_buff[20] = "";
		char * req_str_buff_ptr = req_str_buff;
		int req_str_iterator = 0;
		int content_length = 0;

		for (; *buff_ptr != ' '; buff_ptr++) {
			printf("%c", *buff_ptr);
			if (*buff_ptr != '/') {

				#ifdef DEBUG_URL
					printf("%c", *buff_ptr);
					printf("\n");
				#endif
				req_str_buff[req_str_iterator++] = *buff_ptr;
				content_length++;
			}
		}

		handle_echo_route(req_str_buff_ptr, content_length, client, server);
	}

}


void
handle_echo_route(char *req_string_buff, int content_length, int client, int server)
{
	printf(" request string: %s \n", req_string_buff);
	printf(" content len: %d \n", content_length);

	char response[100] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n";

	int contentLengthStrSize = strlen("Content-Length: " + content_length);
	char contentLengthBuf[50];
	char *contentLengthBuf_ptr = contentLengthBuf;
	int trunc;

	if ( (trunc=snprintf(contentLengthBuf, 50, "Content-Length: %d\r\n\r\n", content_length)) < 0 ) {
		printf("Error converting string: %s \n", strerror(errno));
	}

	strcat(contentLengthBuf, req_string_buff);
	strcat(SUCCESS_RESPONSETYPE_w_CONTENT_, contentLengthBuf);

	send(client, SUCCESS_RESPONSETYPE_w_CONTENT_, strlen(SUCCESS_RESPONSETYPE_w_CONTENT_), 0);
	close(server);

}


enum Route path_to_route(char *path_buff){

	printf("PATH: %s\n", path_buff);

	if (strcmp(path_buff, "echo") == 0) return ECHO;
	if (strcmp(path_buff, "user-agent") == 0) return USER_AGENT;
	if (strcmp(path_buff, " ") == 0) return EMPTY;
	return INVALID_ROUTE;
}


int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

	int server_fd, client_addr_len, client_fd, bytes_read;
	struct sockaddr_in client_addr;
	char buf[BUFSIZ];
	//
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		printf("Socket creation failed: %s...\n", strerror(errno));
		return 1;
	}
	
	// Since the tester restarts your program quite often, setting SO_REUSEADDR
	// ensures that we don't run into 'Address already in use' errors
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEADDR failed: %s \n", strerror(errno));
		return 1;
	}
	
	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(4221),
									 .sin_addr = { htonl(INADDR_ANY) },
									};
	// binding to the socket
	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("Bind failed: %s \n", strerror(errno));
		return 1;
	}
	
	// listen for connection requests , 5 conection requests ALLOWED
	int connection_backlog = 5;
	if (listen(server_fd, connection_backlog) != 0) {
		printf("Listen failed: %s \n", strerror(errno));
		return 1;
	}
	
	printf("Waiting for a client to connect...\n");
	client_addr_len = sizeof(client_addr);
	
	// accept
	if( (client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len)) < 0 ) {
		printf("Request/Connection error: %s \n", strerror(errno));
		return 1;
	}
	printf("Client connected\n");

	//extract
	if ((bytes_read = read(client_fd, buf, BUFSIZ-1)) < 0 ) {
		printf("Socket Read Error: %s \n", strerror(errno));
	}

	printf("bytes read: %d \n", bytes_read);

	char *expected_url_path = "echo"; // 4
	char *buff_ptr = buf;
	char extracted_url_path_buff[100] = "";
	int extracted_path_buff_index = 0;
	int content_length = 0;
	int res;
	
	/** GET /echo/abc HTTP/1.1\r\nHost: localhost:4221\r\nUser-Agent: curl/7.64.1\r\nAccept: */

	for(; *buff_ptr != '/'; buff_ptr++){}


	buff_ptr++;

	if ( *buff_ptr == ' ' ) 
	{
		printf("EMPTY REAQUESTSTRING %c\n", *buff_ptr);

		send(client_fd, SUCCESS_RESPONSE, strlen(SUCCESS_RESPONSE), 0);
		close(server_fd);
	} 
	else 
	{

		// extract url path
		for(; *buff_ptr != '/'; buff_ptr++){
			#ifdef DEBUG
				printf("%c", *buff_ptr);
			#endif

			extracted_url_path_buff[extracted_path_buff_index++] = *buff_ptr;
			content_length++;
		}

		#ifdef DEBUG_EXTRACTED_URL
			printf("\n ### DEBUG ###: url buff : %s\n", extracted_url_path_buff);
		#endif
		

		enum Route extracted_route = path_to_route(extracted_url_path_buff);
		switch(extracted_route) {
			case ECHO: 
				handle_route(buff_ptr, ECHO, client_fd, server_fd);
				break;
			
			case USER_AGENT:
				printf("handle user-agent route");
				handle_route(buff_ptr, USER_AGENT, client_fd, server_fd);
				break;

			case EMPTY:
				printf("handle empty route");
				break;


			default:
				printf("route not supported");
				handle_not_found(client_fd, server_fd);
		}

	}
	
	
	

	return 0;
}
