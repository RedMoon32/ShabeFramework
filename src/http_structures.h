#include <map.h>
#include <zconf.h>

#ifndef STRUCTURES
#define STRUCTURES


#define MAX_REQUEST_LENGTH 1000
#define NUMBER_OF_METHODS 8
#define DATA_LENGTH 10000
#define URL_LENGTH 256
#define URL_NUMBERS 100

#define TRUE 1
#define FALSE 0

#define CONTENT_TYPE "Content-Type"
#define CONTENT_LENGTH "Content-length"
#define TEXT_PLAIN "text/plain"

#define OUT_TAG(mess) ("<html><h4>" mess "</h4></html>")

#define NOT_FOUND_STRING \
OUT_TAG("404 not found")

#define BAD_REQUEST_STRING \
OUT_TAG("400 bad request")

#define SERVER_INTERNAL_STRING \
OUT_TAG("500 server internal error")

typedef map_str_t header_dict;

enum Method {
    GET = 0, HEAD = 1, POST = 2, PUT = 3, DELETE = 4, CONNECT = 5, OPTIONS = 6, TRACE = 7
};

static char http_methods[8][10] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};

typedef struct {
    char request[MAX_REQUEST_LENGTH];
    int client_fd;
    int id;
} Request;


typedef struct {
    enum Method method;
    char url[URL_LENGTH];
    char host[URL_LENGTH];
    header_dict headers;
    char data[DATA_LENGTH];
} HttpRequest;

typedef struct {
    u_int16_t status_code;
    header_dict headers;
    char data[DATA_LENGTH];
} HttpResponse;

#define get_request_header(req, key)   ((char *)map_get_val(&(req)->headers, key))

#define get_response_header(resp, key) ((char *)map_get_val(&(resp)->headers, key))

#define set_response_header(resp, key, value) (map_set(&(resp)->headers, key, value))

#define set_request_header(req, key, value) (map_set(&(req)->headers, key, value))

#define make_response(status_code_, message, resp) \
                                    {(resp->status_code = status_code_);\
                                    if (message != NULL)\
                                    strcpy(resp->data, message);}

#define BAD_RESPONSE(resp) make_response(400, BAD_REQUEST_STRING, resp)
#define NOT_FOUND_RESPONSE(resp) make_response(404, NOT_FOUND_STRING, resp)
#define INTERNAL_ERROR_RESPONSE(resp) make_response(500, SERVER_INTERNAL_STRING, resp)
#define SUCCESS_RESPONSE(resp) make_response(200, NULL, resp)

#endif