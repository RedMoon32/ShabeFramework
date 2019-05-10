#include "HttpStructures.h"


char http_methods[8][10] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};

char *get_request_header(HttpRequest *req, char *key) {
    char **d = map_get(req->headers, key);
    if (d != NULL)
        return *d;
    else
        return NULL;
}

void set_response_header(HttpResponse *resp, char *key, char *value) {
    map_set(resp->headers, key, value);
}

char *get_response_header(HttpRequest *resp, char *key) {
    char **d = map_get(resp->headers, key);
    if (d != NULL)
        return *d;
    else
        return NULL;
}