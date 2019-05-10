//
// Created by rinat on 05.05.19.
//

#include <Dispatcher.h>
#include <HttpStructures.h>
#include <fcntl.h>
#include <stdio.h>

int last_url_number = 0;


void reg_url(char *url, api_url_func *processor) {
    if (last_url_number == URL_NUMBERS)
        return;
    api_url *new_api = &url_patterns[last_url_number];
    strcpy(new_api->url, url);
    new_api->processor = processor;
    last_url_number++;
}

void process_static_url(HttpRequest *req, HttpResponse *resp) {
    if (req->method != GET) {
        resp->status_code = 404;
        return;
    }
    for (int i = 0; i < NUMBER_OF_METHODS; i++) {
        if (strcmp(req->url, url_patterns[i].url) == 0) {
            int file_fd = open(url_patterns[i].path, O_RDONLY);
            char buffer[DATA_LENGTH];
            int st = read(file_fd, buffer, DATA_LENGTH);
            strcpy(resp->data, buffer);
            //printf("Data sent: %s\n", resp->data);
            close(file_fd);
            resp->status_code = 200;
            return;
        }
    }
    resp->status_code = 404;
}

void reg_static_url(char *url, char *path) {
    if (last_url_number == URL_NUMBERS)
        return;
    api_url *new_api = &url_patterns[last_url_number];
    strcpy(new_api->url, url);
    strcpy(new_api->path, path);
    new_api->processor = process_static_url;
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
