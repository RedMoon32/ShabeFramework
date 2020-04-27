//
// Created by rinat on 05.05.19.
//

#include <dispatcher.h>
#include <http_structures.h>
#include <fcntl.h>
#include <stdio.h>
#include <api_funcs.h>
#include <stdlib.h>

/** Function registers new url - some 'ur' will be processes with 'processor' function
 *
 * @param url - new url
 * @param processor - function which will process this url
 */
int register_url(char *url, api_url_func *processor) {
    api_url *new_api = (api_url *) malloc(sizeof(api_url));
    strcpy(new_api->url, url);
    new_api->processor = processor;
    api_funcs_add(new_api);
    return 0;
}

/** Process static url
 *
 * @param req - input http request which contains url path
 * @param resp - output http response with file content in data
 */
void process_static_url(HttpRequest *req, HttpResponse *resp) {

    if (req->method != GET) {
        BAD_RESPONSE(resp);
        return;
    }

    api_url *api_func = api_funcs_get(req->url);

    if (api_func == NULL) {
        NOT_FOUND_RESPONSE(resp);
        return;
    }

    int file_fd = open(api_func->path, O_RDONLY);
    if (file_fd == -1) {
        INTERNAL_ERROR_RESPONSE(resp);
        return;
    }

    char buffer[DATA_LENGTH];
    int st = read(file_fd, buffer, DATA_LENGTH);
    buffer[st] = '\0';
    strcpy(resp->data, buffer);
    close(file_fd);
    SUCCESS_RESPONSE(resp);
}

/** Function to register static url (content of file will be returned on get request)
 *
 * @param url - path of url
 * @param path - path to static file
 */
int register_static_url(char *url, char *path) {
    register_url(url, process_static_url);
    api_url *cur = api_funcs_get(url);
    if (cur == NULL)
        return -1;
    strcpy(cur->path, path);
    return 0;
}

/** Function which returns pointer to function for processing some request
 * if found, otherwise return NULL pointer
 * @param req - input http request
 * @return NULL if not found otherwise pointer to function
 */
api_url_func *get_request_processor(HttpRequest *req) {
    api_url *cur = api_funcs_get(req->url);
    if (cur != NULL)
        return cur->processor;
    return NULL;
}
