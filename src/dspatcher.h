#include "http_structures.h"

#define URL_NUMBERS 100

typedef void (api_url_func)(HttpRequest *, HttpResponse *);


typedef struct api_url_struct {
    char url[URL_LENGTH];
    void *processor;
    char path[URL_LENGTH];
} api_url;


api_url url_patterns[URL_NUMBERS];


void register_url(char *url, api_url_func *processor);

void register_static_url(char *url, char *path);

api_url_func *get_request_processor(HttpRequest *req);