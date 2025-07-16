#ifndef SERVER_HELPERS_H
#define SERVER_HELPERS_H


enum RESPONSE_CODES {
	NOT_FOUND = 404,
	SUCCESS = 200
};

struct Response {
	enum RESPONSE_CODES code;
	char *content_type;
	char *content;
    int content_length;
};

enum ROUTES path_to_route(char *path_buff);
char *build_success_response(struct Response* resp);

#endif