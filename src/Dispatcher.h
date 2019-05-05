#include "HttpStructures.h"

#define URL_NUMBERS 100

typedef void (api_url_func)(HttpRequest *, HttpResponse *);

typedef struct api_url_struct {
    char url[URL_LENGTH];
    api_url_func *processor;
} api_url;


api_url url_patterns[URL_NUMBERS];

void reg_url(char *url, api_url_func *processor);

api_url_func *dispatch(HttpRequest *req);