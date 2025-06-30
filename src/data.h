#include <stdio.h>

#ifndef DATA_H
#define DATA_H

#define DEBUG_EXTRACTED_URL
#define ECHO_ROUTE_LENGTH 4
#define USER_AGENT_HEADER "User-Agent: "


enum Route {
	INVALID_ROUTE = -1,
	EMPTY,
	ECHO,
	USER_AGENT
};

enum Protocol {
    GET,
    POST
};

struct startLine {
	enum Protocol protocol;
	char url_path[BUFSIZ];
	char http_version[10];
};

struct headers {
	/* data */
	char host[50];
	char user_agent[50];
	char accept[100];
	char content_length[BUFSIZ];
};

struct body {
	char data[BUFSIZ];
};

struct Request {
	/* data */
	struct startLine start_line;
	struct headers header;
	struct body body;
};

struct thread_args {
	int client_fd;
	int server_fd;
};

#endif