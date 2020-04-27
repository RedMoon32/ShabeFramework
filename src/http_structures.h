#include <map.h>
#include <zconf.h>

#ifndef STRUCTURES
#define STRUCTURES


#define MAX_REQUEST_LENGTH 1000
#define NUMBER_OF_METHODS 8
#define DATA_LENGTH 10000
#define URL_LENGTH 256
#define PATH_LENGTH 256
#define URL_NUMBERS 100

#define CONTENT_TYPE "Content-Type"
#define NOT_FOUND_STRING "<html><h4> 404 not found </h4></html>"
#define BAD_REQUEST_STRING "<html><h4> 400 bad request </h4></html>"
#define SERVER_INTERNAL_STRING "<html><h4> 500 server internal error </h4></html>"

#define BAD_RESPONSE(resp) make_response(400, BAD_REQUEST_STRING, resp)
#define NOT_FOUND_RESPONSE(resp) make_response(404, NOT_FOUND_STRING, resp)
#define INTERNAL_ERROR_RESPONSE(resp) make_response(500, SERVER_INTERNAL_STRING, resp)
#define SUCCESS_RESPONSE(resp) make_response(200, NULL, resp)

typedef map_str_t header_dict;

enum Method {
    GET = 0, HEAD = 1, POST = 2, PUT = 3, DELETE = 4, CONNECT = 5, OPTIONS = 6, TRACE = 7
};
extern char http_methods[8][10];

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

extern char *get_request_header(HttpRequest *req, char *key);

extern void set_response_header(HttpResponse *resp, char *key, char *value);

extern char *get_response_header(HttpResponse *resp, char *key);

extern void make_response(int status_code, char message[DATA_LENGTH], HttpResponse* resp);

#endif