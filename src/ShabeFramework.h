//
// Created by rinat on 27.04.2020.
//

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <beluckydaf@gmail.com> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Vladislav Smirnov
 * ----------------------------------------------------------------------------
 */

#ifndef _ALIST_H_
#define _ALIST_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    size_t size;
    size_t count;
    void **array;
} array_list;

typedef array_list *p_array_list;

#endif // _ALIST_H_


static p_array_list create_array_list(size_t size) {
    void **array = (void **) malloc(sizeof(void *) * size);
    p_array_list alist = (p_array_list) malloc(sizeof(array_list));
    memset(array, 0, sizeof(void *) * size);
    alist->size = size;
    alist->count = 0;
    alist->array = array;
    return alist;
}

static void delete_array_list(p_array_list alist) {
    free(alist->array);
}

static size_t expand_array_list(p_array_list alist) {
    size_t size = alist->size;
    size_t new_size = size * 2;
    void **array = (void **) malloc(sizeof(void *) * new_size);
    memset(array, 0, sizeof(void *) * new_size);
    memcpy(array, alist->array, sizeof(void *) * size);
    free(alist->array);
    alist->array = array;
    alist->size = new_size;
    return new_size;
}

static int array_list_add(p_array_list alist, void *item) {
    for (int i = 0; i < alist->size; i++) {
        if (alist->array[i] == 0) {
            alist->array[i] = item;
            alist->count++;
            return i;
        }
    }
    int index = (int) ((expand_array_list(alist) / 2) + 1);
    alist->array[index] = item;
    alist->count++;
    return index;
}

static int array_list_remove(p_array_list alist, void *item) {
    for (int i = 0; i < alist->size; i++) {
        if (alist->array[i] == item) {
            alist->array[i] = 0;
            alist->count--;
            return i;
        }
    }
    return -1;
}

static int array_list_remove_at(p_array_list alist, int index) {
    if (alist->array[index] == 0) return -1;
    alist->array[index] = 0;
    alist->count--;
    return index;
}

static int array_list_iter(p_array_list alist) {
    if (alist->count == 0) return -1;
    for (int i = 0; i < alist->size; i++) {
        if (alist->array[i] != 0) return i;
    }
    return -1;
}

static int array_list_next(p_array_list alist, int iter) {
    for (int i = iter + 1; i < alist->size; i++) {
        if (alist->array[i] != 0) return i;
    }
    return -1;
}

static
void *array_list_get(p_array_list alist, int index) {
    if (index > alist->size) return (void *) (-1);
    return alist->array[index];
}

static
void array_list_free_all(p_array_list alist) {
    for (int i = array_list_iter(alist); i != -1; i = array_list_next(alist, i)) {
        void *current = array_list_get(alist, i);
        free(current);
        array_list_remove_at(alist, i);
    }
}

#ifndef SHABEFRAMEWORK_SHABEFRAMEWORK_H
#define SHABEFRAMEWORK_SHABEFRAMEWORK_H

/**
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef MAP_H
#define MAP_H

#include <string.h>

#define MAP_VERSION "0.1.0"

struct map_node_t;
typedef struct map_node_t map_node_t;

typedef struct {
    map_node_t **buckets;
    unsigned nbuckets, nnodes;
} map_base_t;

typedef struct {
    unsigned bucketidx;
    map_node_t *node;
} map_iter_t;


#define map_t(T)\
  struct { map_base_t base; T *ref; T tmp; }


#define map_init(m)\
  memset(m, 0, sizeof(*(m)))


#define map_deinit(m)\
  map_deinit_(&(m)->base)


#define map_get(m, key)\
  ( (m)->ref = map_get_(&(m)->base, key) )


#define map_set(m, key, value)\
  ( (m)->tmp = (value),\
    map_set_(&(m)->base, key, &(m)->tmp, sizeof((m)->tmp)) )


#define map_remove(m, key)\
  map_remove_(&(m)->base, key)

#define map_iter(m)\
  map_iter_()

#define map_next(m, iter)\
  map_next_(&(m)->base, iter)

#define map_free_all(m)\
  map_free_all_(&(m)->base)


typedef map_t(void*) map_void_t;
typedef map_t(char*) map_str_t;
typedef map_t(int) map_int_t;
typedef map_t(char) map_char_t;
typedef map_t(float) map_float_t;
typedef map_t(double) map_double_t;

#endif

/**
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */
//=================================================
#include <stdlib.h>
#include <string.h>

struct map_node_t {
    unsigned hash;
    void *value;
    map_node_t *next;
    /* char key[]; */
    /* char value[]; */
};


static unsigned map_hash(const char *str) {
    unsigned hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) ^ *str++;
    }
    return hash;
}


static map_node_t *map_newnode(const char *key, void *value, int vsize) {
    map_node_t *node;
    int ksize = strlen(key) + 1;
    int voffset = ksize + ((sizeof(void *) - ksize) % sizeof(void *));
    node = malloc(sizeof(*node) + voffset + vsize);
    if (!node) return NULL;
    memcpy(node + 1, key, ksize);
    node->hash = map_hash(key);
    node->value = ((char *) (node + 1)) + voffset;
    memcpy(node->value, value, vsize);
    return node;
}


static int map_bucketidx(map_base_t *m, unsigned hash) {
    /* If the implementation is changed to allow a non-power-of-2 bucket count,
     * the line below should be changed to use mod instead of AND */
    return hash & (m->nbuckets - 1);
}


static void map_addnode(map_base_t *m, map_node_t *node) {
    int n = map_bucketidx(m, node->hash);
    node->next = m->buckets[n];
    m->buckets[n] = node;
}


static int map_resize(map_base_t *m, int nbuckets) {
    map_node_t *nodes, *node, *next;
    map_node_t **buckets;
    int i;
    /* Chain all nodes together */
    nodes = NULL;
    i = m->nbuckets;
    while (i--) {
        node = (m->buckets)[i];
        while (node) {
            next = node->next;
            node->next = nodes;
            nodes = node;
            node = next;
        }
    }
    /* Reset buckets */
    buckets = realloc(m->buckets, sizeof(*m->buckets) * nbuckets);
    if (buckets != NULL) {
        m->buckets = buckets;
        m->nbuckets = nbuckets;
    }
    if (m->buckets) {
        memset(m->buckets, 0, sizeof(*m->buckets) * m->nbuckets);
        /* Re-add nodes to buckets */
        node = nodes;
        while (node) {
            next = node->next;
            map_addnode(m, node);
            node = next;
        }
    }
    /* Return error code if realloc() failed */
    return (buckets == NULL) ? -1 : 0;
}


static map_node_t **map_getref(map_base_t *m, const char *key) {
    unsigned hash = map_hash(key);
    map_node_t **next;
    if (m->nbuckets > 0) {
        next = &m->buckets[map_bucketidx(m, hash)];
        while (*next) {
            if ((*next)->hash == hash && !strcmp((char *) (*next + 1), key)) {
                return next;
            }
            next = &(*next)->next;
        }
    }
    return NULL;
}

static
void map_deinit_(map_base_t *m) {
    map_node_t *next, *node;
    int i;
    i = m->nbuckets;
    while (i--) {
        node = m->buckets[i];
        while (node) {
            next = node->next;
            free(node);
            node = next;
        }
    }
    free(m->buckets);
}

static
void *map_get_(map_base_t *m, const char *key) {
    map_node_t **next = map_getref(m, key);
    return next ? (*next)->value : NULL;
}

static
int map_set_(map_base_t *m, const char *key, void *value, int vsize) {
    int n, err;
    map_node_t **next, *node;
    /* Find & replace existing node */
    next = map_getref(m, key);
    if (next) {
        memcpy((*next)->value, value, vsize);
        return 0;
    }
    /* Add new node */
    node = map_newnode(key, value, vsize);
    if (node == NULL) goto fail;
    if (m->nnodes >= m->nbuckets) {
        n = (m->nbuckets > 0) ? (m->nbuckets << 1) : 1;
        err = map_resize(m, n);
        if (err) goto fail;
    }
    map_addnode(m, node);
    m->nnodes++;
    return 0;
    fail:
    if (node) free(node);
    return -1;
}

static
void map_remove_(map_base_t *m, const char *key) {
    map_node_t *node;
    map_node_t **next = map_getref(m, key);
    if (next) {
        node = *next;
        *next = (*next)->next;
        free(node);
        m->nnodes--;
    }
}

static
map_iter_t map_iter_(void) {
    map_iter_t iter;
    iter.bucketidx = -1;
    iter.node = NULL;
    return iter;
}

static
const char *map_next_(map_base_t *m, map_iter_t *iter) {
    if (iter->node) {
        iter->node = iter->node->next;
        if (iter->node == NULL) goto nextBucket;
    } else {
        nextBucket:
        do {
            if (++iter->bucketidx >= m->nbuckets) {
                return NULL;
            }
            iter->node = m->buckets[iter->bucketidx];
        } while (iter->node == NULL);
    }
    return (char *) (iter->node + 1);
}

static
void map_free_all_(map_base_t *m) {
    const char *key;
    map_iter_t iter = map_iter(m);

    while ((key = map_next_(m, &iter))) {
        void **res = map_get_(m, key);
        if (res != NULL)
            free(*res);
    }
}

//============================================

#include <zconf.h>

#ifndef STRUCTURES
#define STRUCTURES


#define MAX_REQUEST_LENGTH 1000
#define NUMBER_OF_METHODS 8
#define DATA_LENGTH 10000
#define URL_LENGTH 256
#define PATH_LENGTH 256
#define URL_NUMBERS 100

#define CONTENT_TYPE "Content-Type"
#define NOT_FOUND_STRING "<html><h4> 404 not found </h4></html>"
#define BAD_REQUEST_STRING "<html><h4> 400 bad request </h4></html>"
#define SERVER_INTERNAL_STRING "<html><h4> 500 server internal error </h4></html>"

#define BAD_RESPONSE(resp) make_response(400, BAD_REQUEST_STRING, resp)
#define NOT_FOUND_RESPONSE(resp) make_response(404, NOT_FOUND_STRING, resp)
#define INTERNAL_ERROR_RESPONSE(resp) make_response(500, SERVER_INTERNAL_STRING, resp)
#define SUCCESS_RESPONSE(resp) make_response(200, NULL, resp)

typedef map_str_t header_dict;

enum Method {
    GET = 0, HEAD = 1, POST = 2, PUT = 3, DELETE = 4, CONNECT = 5, OPTIONS = 6, TRACE = 7
};
static char http_methods[8][10];

typedef struct {
    char request[MAX_REQUEST_LENGTH];
    int client_fd;
    int id;
} Request;


typedef struct {
    enum Method method;
    char url[URL_LENGTH];
    char host[URL_LENGTH];
    header_dict headers;
    char data[DATA_LENGTH];
} HttpRequest;

typedef struct {
    u_int16_t status_code;
    header_dict headers;
    char data[DATA_LENGTH];
} HttpResponse;

#endif

//=============================================

static char http_methods[8][10] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};

/** Get request header
 *
 * @param req - input http request
 * @param key - output http response
 * @return NULL if not found else string
 */static
char *get_request_header(HttpRequest *req, char *key) {
    char **d = map_get(&req->headers, key);
    if (d != NULL)
        return *d;
    else
        return NULL;
}

/** Set resposne header
 *
 * @param resp- input http response
 * @param key - new string header value
 * @param value - new value
 */static
void set_response_header(HttpResponse *resp, char *key, char *value) {
    map_set(&resp->headers, key, value);
}

/** Get response header
 *
 * @param resp - http response
 * @param key - key string to get
 * @return NULL if not found else string
 */static
char *get_response_header(HttpResponse *resp, char *key) {
    char **d = map_get(&resp->headers, key);
    if (d != NULL)
        return *d;
    else
        return NULL;
}

/**
 * Make response
 * @param status_code - status
 * @param message - data sector
 * @param resp - pointer to response
 */
static
void make_response(int status_code, char message[DATA_LENGTH], HttpResponse *resp) {
    resp->status_code = status_code;
    if (message != NULL)
        strcpy(resp->data, message);
}

// =======================================

#define CR '\r'
#define LF '\n'
#define CRLF "\r\n"

static
HttpRequest *parse_str_to_req(char *req);

static
void parse_resp_to_str(HttpResponse *resp, char *dest);

//====================================

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//============HttpRequest example
// Example of HttpRequest:
// GET /favicon.ico HTTP/1.1
// Host: 127.0.0.1:8080
// Connection: keep-alive
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36 OPR/60.0.3255.70
// Accept: image/webp,image/apng,image/*,*/*;q=0.8
// Referer: http://127.0.0.1:8080/
// Accept-Encoding: gzip, deflate, br
// Accept-Language: en-US,en;q=0.9
// Cookie: csrftoken=hLrpfu90c6NBnEnjkb2FBYNIIzWJ9rxcGVsMsMJ0hY6vjB6EZbwZuzkwpcBxxlF2; sessionid=4x6y6zhkz8a5y99qcv3kh8932wdud4d5
//

//============HttpResponse example
//HTTP/1.1 200
//access-control-expose-headers: X-Frontend
//cache-control: no-store
//content-encoding: gzip
//content-length: 20
//content-type: text/html; charset=windows-1251
//date: Sun, 05 May 2019 06:41:04 GMT
//server: nginx
//status: 200
//strict-transport-security: max-age=15768000
//x-frontend: front605105
//x-powered-by: PHP/3.19177
//
//
//data here


/**Convert http raw string to list of strings (separated by \n\r)
 *
 * @param req - raw http string
 * @param res_req - http request struct(which will be changed,
 * - data will be added to it
 * @return pointer to lines
 */static
char **_break_into_lines(char *req, HttpRequest *res_req) {
#define STR_COUNT 100
    // @todo - replace 100
    char **mas = malloc(STR_COUNT * sizeof(char *));
    memset(mas, 0, STR_COUNT);
    int nline = 0;
    int d_symb = -1; // index represent start of DATA sector (splitted by CRLF CRLF)
    char *start = req;
    mas[0] = start;
    for (int i = 1; i < strlen(req); i++) {
        if (req[i - 1] == CR && req[i] == LF) {

            req[i - 1] = (char) NULL;
            req[i] = (char) NULL;

            if (i < strlen(req) - 2 && req[i + 1] == CR && req[i + 2] == LF) {
                d_symb = i;
                break;
            }
            nline++;
            if (nline == STR_COUNT)
                break;

            mas[nline] = start + i + 1;
        }
    }
    if (d_symb != -1) {
        req[d_symb - 1] = '\0';
        req = req + d_symb + 1;
        strcpy(res_req->data, req);
    }
    return mas;
}

static
/** Parse first string (ex. GET /favicon.ico HTTP/1.1) and get method, url and http version from it
 *
 * @param req - resultant http request struct (method, url will be added to it)
 * @param line - line to parse
 * @return -1 on error, 0 on success
 */
int _parse_declaration(HttpRequest *req, char *line) {
    char *method = strtok(line, " ");
    if (method == NULL)
        return -1;
    char *url = strtok(NULL, " ");
    if (url == NULL)
        return -1;
    char *http = strtok(NULL, " ");
    if (http == NULL || strcmp(http, "HTTP/1.1") != 0)
        return -1;
    for (int i = 0; i < NUMBER_OF_METHODS; i++) {
        if (strcmp(method, http_methods[i]) == 0) {
            req->method = i;
            break;
        }
    }
    if (req->method == -1)
        return -1;
    strcpy(req->url, url);
    return 0;
}

static
/** Parse list of headers and put headers in http req struct's map
 * in form 'key' - 'value'
 *
 * @param req - resultant http request struct
 * @param lines - pointer lines - pointer to start of the header content
 * @return -1 on error, number of line where headers end on success
 */
int _parse_headers(HttpRequest *req, char **lines) {
    char *header, *value;
    map_init(&req->headers);
    int nline = 0;
    while (lines[nline] != NULL && nline < 100) {
        header = strtok(lines[nline], ":");
        value = strtok(NULL, "");
        if (header == NULL || value == NULL) {
            map_deinit(&req->headers);
            return -1;
        }
        map_set(&req->headers, header, value + 1);
        nline++;
    }
    return nline;
}

static
/** Main function - parse raw http request to struct
 *
 * @param req_string - string to parse
 * @return NULL pointer on error, pointer to struct on success
 */
HttpRequest *parse_str_to_req(char *req_string) {
    if (req_string == NULL)
        return NULL;
    char req[MAX_REQUEST_LENGTH];
    strcpy(req, req_string);
    HttpRequest *res = malloc(sizeof(HttpRequest));
    *res = (HttpRequest) {.method = -1, .url = "\0", .host = "\0", .data = "\0"};
    int status;
    char **lines = _break_into_lines(req, res);
    if (lines == NULL) {
        free(res);
        return NULL;
    }
    status = _parse_declaration(res, lines[0]);
    if (status == -1) goto err_parse_out;

    status = _parse_headers(res, lines + 1);
    if (status == -1) goto err_parse_out;

    return res;

    err_parse_out:
    {
        free(lines);
        free(res);
        return NULL;
    }
}

static
/** Convert http response struct to string
 *
 * @param resp - input http response struct
 * @param dest - output string
 */
void parse_resp_to_str(HttpResponse *resp, char *dest) {
    snprintf(dest, DATA_LENGTH, "HTTP/1.1 %u"CRLF, resp->status_code);
    const char *header;
    const char *value;
    map_iter_t iter = map_iter(&resp->headers);
    while ((header = map_next(&resp->headers, &iter))) {
        value = *map_get(&resp->headers, header);
        snprintf(dest + strlen(dest), DATA_LENGTH, "%s: %s"CRLF, header, value);
    }
    strcat(dest, CRLF CRLF);
    strcat(dest, resp->data);
}

//====================================

#ifndef DISPATCHER_H
#define DISPATCHER_H


typedef void (api_url_func)(HttpRequest *, HttpResponse *);

typedef struct api_url_struct {
    char url[URL_LENGTH];
    void *processor;
    char path[URL_LENGTH];
} api_url;

#endif

//====================================

//
// Created by rinat on 05.05.19.
//
static
void api_funcs_init();

static
void api_funcs_add(api_url *new_api);

static
void api_funcs_remove_and_free(api_url *api_to_remove);

static
api_url *api_funcs_get(char *url);

static
void api_funcs_deinit();

//=====================================


#include <stdlib.h>

map_void_t url_patterns;

/**
 * Init api container
 */static
void api_funcs_init() {
    map_init(&url_patterns);
}

/**
 * Replace some url with new function processor
 */static
void api_funcs_add(api_url *new_api) {
    void *cur_past = api_funcs_get(new_api->url);
    api_funcs_remove_and_free(cur_past);
    map_set(&url_patterns, new_api->url, new_api);
}

/**
 * Remove and free memory of some api processor
 */static
void api_funcs_remove_and_free(api_url *api_to_remove) {
    if (api_to_remove != NULL) {
        map_remove(&url_patterns, api_to_remove->url);
        free(api_to_remove);
    }
}

/*
 * Get api processor from the url
 */static
api_url *api_funcs_get(char *url) {
    void **res = map_get(&url_patterns, url);

    if (res != NULL) {
        api_url *api = *res;
        return api;
    }

    return NULL;
}

/**
 * Free all url functions from the container
 */static
void api_funcs_deinit() {
    map_free_all(&url_patterns);
    map_deinit(&url_patterns);
}


//========================

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/** Function registers new url - some 'ur' will be processes with 'processor' function
 *
 * @param url - new url
 * @param processor - function which will process this url
 */static
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
 */static
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
 */static
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
 */static
api_url_func *get_request_processor(HttpRequest *req) {
    api_url *cur = api_funcs_get(req->url);
    if (cur != NULL)
        return cur->processor;
    return NULL;
}

//===================================
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

p_array_list reqs;
int SERVER_PORT;

static
void process_request(Request *req);

static
void server_listen();

static
void server_init();

static
void server_deinit();

//==============================
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

p_array_list reqs;
int master_fd;
int SERVER_PORT;
static int listening = 0;

/** Initialize master tcp socket on predefined port in IPV4 space
 *
 * @return file descriptor of new socket (or will exit if error happened)
 */static
int Socket() {

    struct sockaddr_in address;
    int server_fd;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);


    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

/** Add new request to the queue of incoming requests
 *
 * @param buffer - data received
 * @param new_socket - socket of pending connection
 * @param reqs - array of pointers to request struct
 * @return NULL on error, index of new request in queue on success
 */static
int append_to_requests(char *buffer, int new_socket) {
    Request *new_req = (Request *) malloc(sizeof(Request));
    new_req->client_fd = new_socket;
    memcpy(new_req->request, buffer, strlen(buffer));
    int ind = array_list_add(reqs, new_req);
    new_req->id = ind;
    return ind;
}

/** Function which processes http request:
 * Parses http request -> perform action based on registered urls ->
 * converts http response to string and returns it to client
 * @param req - info about new req(client_fd, req_id, raw_request)
 */static
void process_request(Request *req) {
    HttpRequest *req_res = parse_str_to_req(req->request);
    if (!get_request_header(req_res, CONTENT_TYPE)) {
        map_set(&req_res->headers, CONTENT_TYPE, "text/plain");
    }
    if (req_res == NULL)
        goto out;
    api_url_func *processor = get_request_processor(req_res);

    HttpResponse *resp = malloc(sizeof(HttpResponse));

    map_init(&resp->headers);
    if (processor == NULL) {
        resp->status_code = 404;
        strcpy(resp->data, NOT_FOUND_STRING);
    } else {
        processor(req_res, resp);
    }
    char result[MAX_REQUEST_LENGTH];
    char clength[10];
    sprintf(clength, "%d", strlen(resp->data) + 2);
    map_set(&resp->headers, "Content-length", clength);
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
    array_list_remove_at(reqs, req->id);
    free(req);

}


/**
 * Main listening part
 * @param server_fd - master socket fd (with some port binded to this socket)
 */static
void *server_listen_() {
    int server_fd = master_fd;
    struct sockaddr address;
    int addrlen = sizeof(address);
    int new_socket;
    int valread;

    printf("\n* Server is listening on port 8000\n* Enter 'q' to exit\n\n");

    while (listening) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("In accept");
        }

        char buffer[MAX_REQUEST_LENGTH] = {0};
        valread = read(new_socket, buffer, MAX_REQUEST_LENGTH);
        if (valread == 0 || valread == -1) {
            close(new_socket);
            continue;
        }
#ifdef TESTING
        printf("%s - new request\n", buffer);
#endif
        int last_req = append_to_requests(buffer, new_socket);
        process_request(array_list_get(reqs, last_req));
    }
}

static
void server_deinit() {
    listening = 0;
    array_list_free_all(reqs);
    delete_array_list(reqs);
    api_funcs_deinit();
    close(master_fd);
}

/**
 * Ask for out and if 'q' symbol then exit
 */static
void *ask_out() {
    while (1) {
        char c;
        scanf("%c", &c);
        if (c == 'q') {
            printf("Closing server ...\n");
            server_deinit();
            exit(0);
        }

    }
}

/**
 * Function to start listening in new thread
 */static
void server_listen() {
    listening = 1;
    pthread_t listen_thread, ask_out_thread;
    pthread_create(&listen_thread, NULL, server_listen_, NULL);
    pthread_create(&ask_out_thread, NULL, ask_out, NULL);
    pthread_join(listen_thread, NULL);
}

/**
 * Function to start server on 8000 port
 */static
void server_init() {
    SERVER_PORT = 8000;
    reqs = create_array_list(100);
    api_funcs_init();
    master_fd = Socket();
}

#endif //SHABEFRAMEWORK_SHABEFRAMEWORK_H
