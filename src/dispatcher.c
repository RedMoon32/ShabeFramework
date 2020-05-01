//
// Created by rinat on 05.05.19.
//

#include <dispatcher.h>
#include <http_structures.h>
#include <fcntl.h>
#include <stdio.h>
#include <api_funcs.h>
#include <stdlib.h>
#include <parser.h>
#include <dirent.h>
#include <str_funcs.h>

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


/** Function which processes http request:
 * Parses http request -> perform action based on registered urls ->
 * converts http response to string and returns it to client
 * @param req - info about new req(client_fd, req_id, raw_request)
 */
void process_request(Request *req, p_array_list *reqs) {
    HttpRequest *req_res = parse_str_to_req(req->request);
    if (!get_request_header(req_res, CONTENT_TYPE)) {
        set_request_header(req_res, CONTENT_TYPE, TEXT_PLAIN);
    }

    if (req_res == NULL)
        goto out;

    api_url_func *processor = get_request_processor(req_res);

    HttpResponse *resp = malloc(sizeof(HttpResponse));

    map_init(&resp->headers);
    if (processor == NULL) {
        NOT_FOUND_RESPONSE(resp);
    } else {
        processor(req_res, resp);
    }
    char result[MAX_REQUEST_LENGTH];
    char clength[10];
    sprintf(clength, "%lu", strlen(resp->data) + 2);
    set_response_header(resp, CONTENT_LENGTH, clength);
    parse_resp_to_str(resp, result);
    strcat(result, "\0");
    int st = write(req->client_fd, result, strlen(result) + 1);
#ifdef TESTING
    printf("Response %s:\n",result);
#endif
    printf("<- %s %s %d\n", http_methods[req_res->method], req_res->url, resp->status_code);

    map_deinit(&resp->headers);
    free(resp);
    map_deinit(&req_res->headers);
    free(req_res);

    out:
    st = close(req->client_fd);
    array_list_remove_at(*reqs, req->id);
    free(req);

}

/**
 * Map all static files from folder to urls
 *
 * @param path - path to static folder
 * @return 0/-1 on success/error
 */
int set_static_folder(char *path){
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {

            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;

            char url[PATH_MAX];
            char file_path[PATH_MAX];
            char file_corr_path[PATH_MAX];

            concat_str(STATIC_PREFIX, dir->d_name, url);
            realpath(concat_str(path, dir->d_name, file_path), file_corr_path);

            register_static_url(url, file_corr_path);
        }
        closedir(d);
    } else
        return -1;

    return 0;
}