//
// Created by rinat on 02.05.2020.
//

#include <acutest.h>
#include <http_server.h>
#include <parser.h>
#include <dispatcher.h>
#include <api_funcs.h>

#define TESTING

char *TEST_REQ1 = "GET /favicon.ico HTTP/1.1\r\n"
                  "Host: 127.0.0.1:8080\r\n"
                  "Connection: keep-alive\r\n"
                  "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36 OPR/60.0.3255.70\r\n"
                  "Accept: image/webp,image/apng,image/*,*/*;q=0.8\r\n"
                  "Referer: http://127.0.0.1:8080/\r\n"
                  "Accept-Encoding: gzip, deflate, br\r\n"
                  "Accept-Language: en-US,en;q=0.9\r\n"
                  "Cookie: csrftoken=hLrpfu90c6NBnEnjkb2FBYNIIzWJ9rxcGVsMsMJ0hY6vjB6EZbwZuzkwpcBxxlF2; sessionid=4x6y6zhkz8a5y99qcv3kh8932wdud4d5\r\n\r\nDATA HERE";

char *TEST_RESP1 = "HTTP/1.1 200\r\n"
                   "content-type: text/html; charset=windows-1251\r\n"
                   "content-encoding: gzip\r\n"
                   "cache-control: no-store\r\n"
                   "access-control-expose-headers: X-Frontend\r\n"
                   "\r\n"
                   "<html><h1>Kek</h1></html>";

char *TEST_RESP2 = "HTTP/1.1 200\r\n"
                   "Content-length: 42\r\n"
                   "content_type: text/html\r\n"
                   "cache-control: no-store\r\n"
                   "\r\n"
                   "<html><h1>Hello from server!</h1></html>";

void helper_test_home_page(HttpRequest *req, HttpResponse *resp) {
    if (req->method == GET) {
        resp->status_code = 200;
        set_response_header(resp, "cache-control", "no-store");
        set_response_header(resp, "content_type", "text/html");
        strcpy(resp->data, "<html><h1>Hello from server!</h1></html>");
    }
}

void test_api_funcs(void) {
    api_url *api1 = malloc(sizeof(api_url));
    strcpy(api1->url, "AA");
    api1->processor = (api_url_func *) 1;

    api_funcs_init();
    api_funcs_add(api1);

    api_url *res = api_funcs_get("AA");

    TEST_CHECK(res != NULL);
    TEST_CHECK(res->processor == (api_url_func *) 1);

    api_funcs_remove_and_free(api1);
    TEST_CHECK(api_funcs_get("AA") == NULL);

    api_funcs_deinit();
}

void test_request_parse(void) {
    HttpRequest *req = parse_str_to_req(TEST_REQ1);
    TEST_CHECK(req != NULL);
    TEST_CHECK(req->method == GET);
    TEST_CHECK(strcmp(req->url, "/favicon.ico") == 0);
    TEST_CHECK(strcmp(get_request_header(req, "Host"), "127.0.0.1:8080") == 0);
    TEST_CHECK(strcmp(get_request_header(req, "Connection"), "keep-alive") == 0);
    TEST_CHECK(strcmp(get_request_header(req, "User-Agent"),
                      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36 OPR/60.0.3255.70") ==
               0);
    TEST_CHECK(strcmp(get_request_header(req, "Accept-Encoding"), "gzip, deflate, br") == 0);
    TEST_CHECK(strcmp(get_request_header(req, "Cookie"),
                      "csrftoken=hLrpfu90c6NBnEnjkb2FBYNIIzWJ9rxcGVsMsMJ0hY6vjB6EZbwZuzkwpcBxxlF2; sessionid=4x6y6zhkz8a5y99qcv3kh8932wdud4d5") ==
               0);
    TEST_CHECK(strcmp(req->data, "DATA HERE") == 0);
    free(req);
}

void test_create_response_string(void) {

    HttpResponse resp;
    resp = (HttpResponse) {.status_code = 200, .data = "<html><h1>Kek</h1></html>"};
    map_init(&resp.headers);
    map_set(&resp.headers, "access-control-expose-headers", "X-Frontend");
    map_set(&resp.headers, "cache-control", "no-store");
    map_set(&resp.headers, "content-encoding", "gzip");
    map_set(&resp.headers, "content-type", "text/html; charset=windows-1251");
    char data[MAX_REQUEST_LENGTH];
    parse_resp_to_str(&resp, data);
    TEST_CHECK(strcmp(data, TEST_RESP1) == 0);
    TEST_MSG("Expected: %s\n\n", TEST_RESP1);
    TEST_MSG("Produced: %s\n\n", data);

    map_deinit(&resp.headers);
}

void test_process_request(void) {
    int st = putenv("TESTING=TRUE");

    p_array_list reqs_test = create_array_list(100);
    api_funcs_init();

    char temp[PATH_MAX] = "tempRespXXXXXX";
    int resp_file = mkstemp(temp);
    TEST_ASSERT(resp_file != -1);
    Request *req = (Request *) malloc(sizeof(Request));
    *req = (Request) {.id = 0, .request = "GET /home HTTP/1.1\r\n\r\n", .client_fd = resp_file};
    register_url("/home", helper_test_home_page);

    array_list_add(reqs_test, &reqs);
    process_request(req, reqs_test);
    char buff[MAX_REQUEST_LENGTH];
    lseek(resp_file, 0, SEEK_SET);
    int l = read(resp_file, buff, MAX_REQUEST_LENGTH);
    TEST_ASSERT(l != -1);
    buff[l] = '\0';

    TEST_CHECK(strcmp(buff, TEST_RESP2) == 0);
    TEST_MSG("Expected: %s\n\n", TEST_RESP2);
    TEST_MSG("Produced: %s\n\n", buff);


    delete_array_list(reqs_test);
    api_funcs_deinit();
    close(resp_file);
}

TEST_LIST = {
        {"Api functionality", test_api_funcs},
        {"Request Parse", test_request_parse},
        {"Response String", test_create_response_string},
        {"Process Request", test_process_request},
        {NULL, NULL}
};