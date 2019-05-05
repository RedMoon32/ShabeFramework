#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <Parser.h>

void test1() {
    char *req1 = "GET /favicon.ico HTTP/1.1\n"
                 "Host: 127.0.0.1:8080\n"
                 "Connection: keep-alive\n"
                 "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36 OPR/60.0.3255.70\n"
                 "Accept: image/webp,image/apng,image/*,*/*;q=0.8\n"
                 "Referer: http://127.0.0.1:8080/\n"
                 "Accept-Encoding: gzip, deflate, br\n"
                 "Accept-Language: en-US,en;q=0.9\n"
                 "Cookie: csrftoken=hLrpfu90c6NBnEnjkb2FBYNIIzWJ9rxcGVsMsMJ0hY6vjB6EZbwZuzkwpcBxxlF2; sessionid=4x6y6zhkz8a5y99qcv3kh8932wdud4d5\n\nDATA HERE";
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

struct s2 {
    char data[20];
};

struct s1 {
    char a;
    char b;
    char c;
    char d;
};

void test2() {

}

int main() {
    struct s2 _s2_ex = (struct s2) {.data="kekich"};
    printf("%s \n", _s2_ex.data);
    struct s1 *_s1_ex;
    _s1_ex = (struct s1 *) &_s2_ex;
    printf("%d \n", strlen(&(_s1_ex->a)));
    printf("%c %c %c %c\n", _s1_ex->a, _s1_ex->b, _s1_ex->c, _s1_ex->d);
    //test1();
    //test2();
}