#include <stdio.h>

#ifndef DATA_H
#define DATA_H

#define DEBUG_EXTRACTED_URL
#define ECHO_ROUTE_LENGTH 4
#define USER_AGENT_HEADER "User-Agent: "
#define GZIP "gzip"

typedef char * String;

typedef enum {
	INVALID_ROUTE = -1,
	EMPTY,
	ECHO,
	FILES,
	USER_AGENT
} ROUTES;

typedef enum {
	gzip,
	UNSUPPORTED_SCHEME

} COMPRESSION_SCHEMES;

typedef enum {
	BAD_REQUEST = 400,
	CREATED = 201,
	NOT_FOUND = 404,
	SUCCESS = 200,
	SERVER_ERROR = 500
} RESPONSE_CODES;

typedef enum {
    GET,
    POST,
	UNSUPPORTED_METHOD
} REQUEST_METHODS;

typedef struct {
	String host;
	String user_agent;
	String accept;
	String accept_encoding;
	String content_type;
	int  content_length;
} Headers;

typedef struct {
	RESPONSE_CODES code;
	String content_type;
	String content;
	String content_encoding;
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