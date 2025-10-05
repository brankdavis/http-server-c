#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "data.h"
#include "server/server.h"

int 
main() 
{
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

	int server_fd, client_addr_len, client_fd, bytes_read;
	struct sockaddr_in client_addr;
	pthread_t thread;
	
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


	while(1) {

		// accept
		if( (client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len)) < 0 ) {
			printf("Request/Connection error: %s \n", strerror(errno));
			return 1;
		}

		Server_args *args = malloc(sizeof(Server_args));

		if (args == NULL) {
			perror("malloc failed");
			return 1;
		}


		args->client_fd = client_fd;
		args-> server_fd = server_fd;


		if (pthread_create(&thread, NULL, server, args) != 0) {
			printf("Thread error %s \n", strerror(errno));
			//close(client_fd);
			return 1;
		}

		pthread_detach(thread);

	}
	
	return 0;
}
