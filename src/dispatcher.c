//
// Created by rinat on 05.05.19.
//

#include <dispatcher.h>
#include <http_structures.h>
#include <fcntl.h>
#include <stdio.h>

/**
 * Get api func from url patterns by key
 * @param url - key
 */
api_url* get_api_func(char *url){
    void** res = map_get(&url_patterns, url);
    if (res != NULL){
        return (api_url*) *res;
    }
    return NULL;
}

/** Function registers new url - some 'ur' will be processes with 'processor' function
 *
 * @param url - new url
 * @param processor - function which will process this url
 */
int register_url(char *url, api_url_func *processor) {
    api_url *new_api = (api_url *) malloc(sizeof(api_url));
    strcpy(new_api->url, url);
    new_api->processor = processor;
    void* cur_past = get_api_func(url);
    if (cur_past != NULL) {
        map_remove(&url_patterns, url);
        free(cur_past);
    }
    map_set(&url_patterns, url, new_api);

    return 0;
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

    api_url *api_func = get_api_func(req->url);
    if (api_func == NULL) {
        resp->status_code = 404;
        return;
    }

    int file_fd = open(api_func->path, O_RDONLY);
    if (file_fd == -1) {
        resp->status_code = 500;
        return;
    }

    char buffer[DATA_LENGTH];
    int st = read(file_fd, buffer, DATA_LENGTH);
    buffer[st] = '\0';
    strcpy(resp->data, buffer);
    close(file_fd);
    resp->status_code = 200;

}

/** Function to register static url (content of file will be returned on get request)
 *
 * @param url - path of url
 * @param path - path to static file
 */
int register_static_url(char *url, char *path) {
    register_url(url, process_static_url);
    api_url *cur = get_api_func(url);
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
    api_url *cur = get_api_func(req->url);
    if (cur!=NULL)
        return cur->processor;
    return NULL;
}
