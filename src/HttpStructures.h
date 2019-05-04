#include <map.h>

#define MAX_REQUEST_LENGTH 1000
#define NUMBER_OF_METHODS 8
#define DATA_LENGTH 10000
#define URL_LENGTH 256

#ifndef STRUCTURES
#define STRUCTURES

extern enum Method {
    GET = 0, HEAD = 1, POST = 2, PUT = 3, DELETE = 4, CONNECT = 5, OPTIONS = 6, TRACE = 7
};
extern char http_methods[8][10];

typedef struct {
    char request[MAX_REQUEST_LENGTH];
    int clientfd;
    int id;
} Request;


typedef struct {
    enum Method method;
    char url[URL_LENGTH];
    char host[URL_LENGTH];
    map_str_t *headers;
    char data[DATA_LENGTH];
} HttpRequest;

typedef struct {

} HttpResponse;

extern char *get_request_header(HttpRequest *req, char *key);

#endif