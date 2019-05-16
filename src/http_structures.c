#include "http_structures.h"


char http_methods[8][10] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};

/** Get request header
 *
 * @param req - input http request
 * @param key - output http response
 * @return NULL if not found else string
 */
char *get_request_header(HttpRequest *req, char *key) {
    char **d = map_get(req->headers, key);
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
 */
void set_response_header(HttpResponse *resp, char *key, char *value) {
    map_set(resp->headers, key, value);
}

/** Get response header
 *
 * @param resp - http response
 * @param key - key string to get
 * @return NULL if not found else string
 */
char *get_response_header(HttpResponse *resp, char *key) {
    char **d = map_get(resp->headers, key);
    if (d != NULL)
        return *d;
    else
        return NULL;
}