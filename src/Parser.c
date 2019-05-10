#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <map.h>
#include "HttpStructures.h"
#include "Parser.h"

#define PARSE_OK 0

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

char **_break_into_lines(char *req, HttpRequest *res_req) {
    char **mas = malloc(100 * sizeof(char *));
    memset(mas, NULL, 100);
    int nline = 0;
    int d_symb = -1;
    for (int i = 1; i < strlen(req); i++) {
        if (req[i] == '\n' && req[i - 1] == '\r' && req[i - 2] == '\n' && req[i - 3] == '\r') {
            d_symb = i;
            break;
        }
    }
    if (d_symb == -1) return NULL;
    req[d_symb - 1] = '\0';
    char *line = strtok(req, "\n\r");
    while (line != NULL) {
        mas[nline] = line;
        line = strtok(NULL, "\n\r");
        ++nline;
    }
    req = req + d_symb + 1;
    strcpy(res_req->data, req);
    return mas;
}

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

int _parse_headers(HttpRequest *req, char **lines) {
    char *header, *value;
    req->headers = malloc(sizeof(map_str_t));
    map_init(req->headers);
    int nline = 0;
    while (lines[nline] != NULL && nline < 100) {
        header = strtok(lines[nline], ":");
        value = strtok(NULL, "");
        if (header == NULL || value == NULL) {
            map_deinit(req->headers);
            return -1;
        }
        map_set(req->headers, header, value + 1);
        nline++;
    }
    return nline;
}


HttpRequest *parse_str_to_req(char *req_string) {
    if (req_string == NULL)
        return NULL;
    char req[MAX_REQUEST_LENGTH];
    strcpy(req, req_string);
    HttpRequest *res = malloc(sizeof(HttpRequest));
    *res = (HttpRequest) {.method = -1, .url = "\0", .host = "\0", .headers = NULL, .data = "\0"};
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

/*
 * HTTP/1.1 200
access-control-expose-headers: X-Frontend
cache-control: no-store
content-encoding: gzip
content-length: 20
content-type: text/html; charset=windows-1251
date: Sun, 05 May 2019 06:41:04 GMT
server: nginx
status: 200
strict-transport-security: max-age=15768000
x-frontend: front605105
x-powered-by: PHP/3.19177
 */
void parse_resp_to_str(HttpResponse *resp, char *dest) {
    snprintf(dest, DATA_LENGTH, "HTTP/1.1 %u\r\n", resp->status_code);
    const char *header;
    const char *value;
    map_iter_t iter = map_iter(resp->headers);
    while ((header = map_next(resp->headers, &iter))) {
        value = *map_get(resp->headers, header);
        snprintf(dest + strlen(dest), DATA_LENGTH, "%s: %s\r\n", header, value);
    }
    strcat(dest, "\r\n\r\n");
    strcat(dest, resp->data);
}