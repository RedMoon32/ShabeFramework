#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <parser.h>

void test1() {
    char *req1 = "GET /favicon.ico HTTP/1.1\r\n"
                 "Host: 127.0.0.1:8080\r\n"
                 "Connection: keep-alive\r\n"
                 "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36 OPR/60.0.3255.70\r\n"
                 "Accept: image/webp,image/apng,image/*,*/*;q=0.8\r\n"
                 "Referer: http://127.0.0.1:8080/\r\n"
                 "Accept-Encoding: gzip, deflate, br\r\n"
                 "Accept-Language: en-US,en;q=0.9\r\n"
                 "Cookie: csrftoken=hLrpfu90c6NBnEnjkb2FBYNIIzWJ9rxcGVsMsMJ0hY6vjB6EZbwZuzkwpcBxxlF2; sessionid=4x6y6zhkz8a5y99qcv3kh8932wdud4d5\r\n\r\nDATA HERE";
    HttpRequest *req = parse_str_to_req(req1);
    assert(req != NULL);
    assert(req->method == GET);
    assert(strcmp(req->url, "/favicon.ico") == 0);
    assert(strcmp(get_request_header(req, "Host"), "127.0.0.1:8080") == 0);
    assert(strcmp(get_request_header(req, "Connection"), "keep-alive") == 0);
    assert(strcmp(get_request_header(req, "User-Agent"),
                  "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36 OPR/60.0.3255.70") ==
           0);
    assert(strcmp(get_request_header(req, "Accept-Encoding"), "gzip, deflate, br") == 0);
    assert(strcmp(get_request_header(req, "Cookie"),
                  "csrftoken=hLrpfu90c6NBnEnjkb2FBYNIIzWJ9rxcGVsMsMJ0hY6vjB6EZbwZuzkwpcBxxlF2; sessionid=4x6y6zhkz8a5y99qcv3kh8932wdud4d5") ==
           0);
    assert(strcmp(req->data, "DATA HERE") == 0);
    printf("Test 1 success\n");
}

void test2() {
    HttpResponse *resp = malloc(sizeof(HttpResponse));
    *resp = (HttpResponse) {.status_code = 200, .data = "<html><h1>Kek</h1></html", .headers = NULL};
    resp->headers = malloc(sizeof(map_str_t));
    map_init(resp->headers);
    map_set(resp->headers, "access-control-expose-headers", "X-Frontend");
    map_set(resp->headers, "cache-control", "no-store");
    map_set(resp->headers, "content-encoding", "gzip");
    map_set(resp->headers, "content-type", "text/html; charset=windows-1251");
    char data[MAX_REQUEST_LENGTH];
    parse_resp_to_str(resp, data);
    assert(strcmp(data, "HTTP/1.1 200\r\n"
                        "content-type: text/html; charset=windows-1251\r\n"
                        "content-encoding: gzip\r\n"
                        "cache-control: no-store\r\n"
                        "access-control-expose-headers: X-Frontend\r\n"
                        "\r\n"
                        "\r\n"
                        "<html><h1>Kek</h1></html") == 0);
    printf("Test 2 success");
}


int main() {
    test1();
    test2();
}