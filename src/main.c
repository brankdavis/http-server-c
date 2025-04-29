#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

// #define DEBUG


int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	printf("Logs from your program will appear here!\n");

	// Uncomment this block to pass the first stage
	//
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

	printf("%s\n", buf);

	int i;
	for (i = 0; buf[i] != '/'; i++) {
		#ifdef DEBUG
			printf("%c\n", buf[i]);
		#endif
	}
	
	// only handle '/' request path
	if (buf[i+1] != ' '){
		char *response = "HTTP/1.1 404 Not Found\r\n\r\n";
		send(client_fd, response, strlen(response), 0);
		close(server_fd);
		return 0;
	}

	// respond
	char *response = "HTTP/1.1 200 OK\r\n\r\n";
	send(client_fd, response, strlen(response), 0);
	close(server_fd);

	return 0;
}
