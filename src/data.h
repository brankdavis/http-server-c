#include <stdio.h>

#ifndef DATA_H
#define DATA_H

#define DEBUG_EXTRACTED_URL
#define ECHO_ROUTE_LENGTH 4
#define USER_AGENT_HEADER "User-Agent: "

typedef char* String;

enum ROUTES {
	INVALID_ROUTE = -1,
	EMPTY,
	ECHO,
	FILES,
	USER_AGENT
};

enum RESPONSE_CODES {
	BAD_REQUEST = 400,
	CREATED = 201,
	NOT_FOUND = 404,
	SUCCESS = 200,
	SERVER_ERROR = 500
};

typedef enum {
    GET,
    POST,
	UNSUPPORTED_METHOD
} REQUEST_METHODS;

typedef struct {
	char *host;
	char *user_agent;
	char *accept;
	char *content_type;
	int  content_length;
} Headers;

typedef struct {
	enum RESPONSE_CODES code;
	char *content_type;
	char *content;
	int content_length;
} Response;

typedef struct {
	REQUEST_METHODS http_method;
	Headers *headers;
	String url;
	String version;
	String request_body;
} Request;

typedef struct {
	int client_fd;
	int server_fd;
} Server_args;

#endif