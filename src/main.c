#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

// #define DEBUG_
// #define DEBUG_URL
// #define DEBUG_EXTRACTED_URL
#define TEST
#define PROD


int
calculate_content_length(int intitial_content_length, char *expected_url_path) {
	return intitial_content_length - strlen(expected_url_path);
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
	//fwrite(buf, bytes_read, bytes_read,stdout);

	//printf("%s\n", buf);
	// Get request Line

	char *expected_url_path = "echo"; // 4
	char *buff_ptr = buf;
	char extracted_url_path_buff[100] = "";
	int extracted_path_buff_index = 0;
	int content_length = 0;
	int res;
	
	/** GET /echo/abc HTTP/1.1\r\nHost: localhost:4221\r\nUser-Agent: curl/7.64.1\r\nAccept: */

	//printf("printing....\n");
	for(; *buff_ptr != '/'; buff_ptr++){
		#ifdef DEBUG
			printf("%c", *buff_ptr);
		#endif
	}

	buff_ptr++;
	
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

	if ( (res = strcmp(expected_url_path, extracted_url_path_buff) != 0) ) 
	{
		char response[30] = "HTTP/1.1 404 Not Found\r\n\r\n";
		send(client_fd, response, strlen(response), 0);
		close(server_fd);
	}
	else {

		char req_str_buff[20] = "";
		int req_str_iterator = 0;

		for (; *buff_ptr != ' '; buff_ptr++) {
		if (*buff_ptr != '/') {

			#ifdef DEBUG_URL
				printf("%c", *buff_ptr);
				printf("\n");
			#endif
			content_length++;
		}

		if (content_length > 4) {
			#ifdef DEBUG_URL
				printf("### content_len > 4\n");
				printf("%c", *buff_ptr);
				printf("\n");
			#endif

			req_str_buff[req_str_iterator++] = *buff_ptr;
		}
		}


		#ifdef DEBUG_URL
			printf("\n ### DEBUG ###: requ str buff : %s\n", req_str_buff);
		#endif

		content_length = calculate_content_length(content_length, expected_url_path);

		#ifdef DEBUG_URL
			printf("\n ### DEBUG ###: Content Lengh: %d\n", content_length);
		#endif

		char response[100] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n";

		int contentLengthStrSize = strlen("Content-Length: " + content_length);
		char contentLengthBuf[50];
		char *contentLengthBuf_ptr = contentLengthBuf;
		int trunc;
		

		#ifdef DEBUG_URL
			printf("\n  ### DEBUG ### Content Lengh Size: %d\n", contentLengthStrSize);
		#endif

		if ( (trunc=snprintf(contentLengthBuf, 50, "Content-Length: %d\r\n\r\n", content_length)) < 0 ) {
			printf("Error converting string: %s \n", strerror(errno));
		}

		#ifdef DEBUG_URL
			printf("### DEBUG ### sprintf returned value (trunc): %d\n", trunc);
			printf("### DEBUG ### content length buffer: %s\n",contentLengthBuf);
		#endif

		strcat(contentLengthBuf, req_str_buff);
		strcat(response, contentLengthBuf);

		#ifdef DEBUG_URL
			printf("\n  ### DEBUG ### final response: %s\n", response);
		#endif
		
		send(client_fd, response, strlen(response), 0);
		close(server_fd);
	}
	
	

	return 0;
}
