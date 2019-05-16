//
// Created by rinat on 05.05.19.
//

#include <dispatcher.h>
#include <http_structures.h>
#include <fcntl.h>
#include <stdio.h>


/** Function registers new url - some 'ur' will be processes with 'processor' function
 *
 * @param url - new url
 * @param processor - function which will process this url
 */
int register_url(char *url, api_url_func *processor) {
    api_url *new_api = (api_url *) malloc(sizeof(api_url));
    strcpy(new_api->url, url);
    new_api->processor = processor;
    int ind = array_list_add(url_patterns, new_api);
    return ind;
}


/** Process static url
 *
 * @param req - input http request which contains url path
 * @param resp - output http response with file content in data
 */
void process_static_url(HttpRequest *req, HttpResponse *resp) {
    if (req->method != GET) {
        resp->status_code = 400;
        return;
    }
    for (int i = array_list_iter(url_patterns); i != -1; i = array_list_next(url_patterns, i)) {
        api_url *cur = array_list_get(url_patterns, i);
        if (strcmp(req->url, cur->url) == 0) {
            int file_fd = open(cur->path, O_RDONLY);
            if (file_fd == -1)
                return;
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
int register_static_url(char *url, char *path) {
    int new = register_url(url, process_static_url);
    api_url *cur = array_list_get(url_patterns, new);
    strcpy(cur->path, path);
    return new;
}

/** Function which returns pointer to function for processing some request
 * if found, otherwise return NULL pointer
 * @param req - input http request
 * @return NULL if not found otherwise pointer to function
 */
api_url_func *get_request_processor(HttpRequest *req) {
    for (int i = array_list_iter(url_patterns); i != -1; i = array_list_next(url_patterns, i)) {
        api_url *cur = array_list_get(url_patterns, i);
        if (strcmp(cur->url, req->url) == 0) {
            return cur->processor;
        }
    }
    return NULL;
}
