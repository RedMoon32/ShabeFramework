//
// Created by rinat on 05.05.19.
//

#include <Dispatcher.h>
#include <HttpStructures.h>

int last_url_number = 0;

void reg_url(char *url, api_url_func *processor) {
    if (last_url_number == URL_NUMBERS)
        return;
    api_url *new_api = &url_patterns[last_url_number];
    strcpy(new_api->url, url);
    new_api->processor = processor;
    last_url_number++;
}

api_url_func *dispatch(HttpRequest *req) {
    for (int i = 0; i < last_url_number; i++) {
        if (strcmp(url_patterns[i].url, req->url) == 0) {
            return url_patterns[i].processor;
        }
    }
    return NULL;
}