//
// Created by rinat on 05.05.19.
//

#include <dspatcher.h>
#include <http_structures.h>
#include <fcntl.h>
#include <stdio.h>

int last_url_number = 0;

/** Function registers new url - some 'ur' will be processes with 'processor' function
 *
 * @param url - new url
 * @param processor - function which will process this url
 */
void register_url(char *url, api_url_func *processor) {
    if (last_url_number == URL_NUMBERS)
        return;
    api_url *new_api = &url_patterns[last_url_number];
    strcpy(new_api->url, url);
    new_api->processor = processor;
    last_url_number++;
}

/** Process static url
 *
 * @param req - input http request which contains url path
 * @param resp - output http response with file content in data
 */
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
            buffer[st] = '\0';
            strcpy(resp->data, buffer);
            close(file_fd);
            resp->status_code = 200;
            return;
        }
    }
    resp->status_code = 404;
}

/** Function to register static url (content of file will be returned on get request)
 *
 * @param url - path of url
 * @param path - path to static file
 */
void register_static_url(char *url, char *path) {
    if (last_url_number == URL_NUMBERS)
        return;
    api_url *new_api = &url_patterns[last_url_number];
    strcpy(new_api->url, url);
    strcpy(new_api->path, path);
    new_api->processor = process_static_url;
    last_url_number++;
}

/** Function which returns pointer to function for processing some request
 * if found, otherwise return NULL pointer
 * @param req - input http request
 * @return NULL if not found otherwise pointer to function
 */
api_url_func *get_request_processor(HttpRequest *req) {
    for (int i = 0; i < last_url_number; i++) {
        if (strcmp(url_patterns[i].url, req->url) == 0) {
            return url_patterns[i].processor;
        }
    }
    return NULL;
}
