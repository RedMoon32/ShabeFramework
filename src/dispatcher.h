#include <alist.h>
#include "http_structures.h"

#define URL_NUMBERS 100

#ifndef DISPATCHER_H
#define DISPATCHER_H

typedef void (api_url_func)(HttpRequest *, HttpResponse *);


typedef struct api_url_struct {
    char url[URL_LENGTH];
    void *processor;
    char path[URL_LENGTH];
} api_url;

map_void_t url_patterns;

int register_url(char *url, api_url_func *processor);

int register_static_url(char *url, char *path);

api_url_func *get_request_processor(HttpRequest *req);

#endif