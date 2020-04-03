#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <parser.h>
#include <dispatcher.h>
#include <fcntl.h>
#include <http_server.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LOCALHOST "127.0.0.1"

void test_parse() {
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
    printf("%s \n", get_request_header(req, "Host"));
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
    printf("=== Test 1 success\n");
    free(req);
}

void test_resp_string() {
    HttpResponse *resp = malloc(sizeof(HttpResponse));
    *resp = (HttpResponse) {.status_code = 200, .data = "<html><h1>Kek</h1></html>"};

    map_init(&resp->headers);
    map_set(&resp->headers, "access-control-expose-headers", "X-Frontend");
    map_set(&resp->headers, "cache-control", "no-store");
    map_set(&resp->headers, "content-encoding", "gzip");
    map_set(&resp->headers, "content-type", "text/html; charset=windows-1251");
    char data[MAX_REQUEST_LENGTH];
    parse_resp_to_str(resp, data);
    assert(strcmp(data, "HTTP/1.1 200\r\n"
                        "content-type: text/html; charset=windows-1251\r\n"
                        "content-encoding: gzip\r\n"
                        "cache-control: no-store\r\n"
                        "access-control-expose-headers: X-Frontend\r\n"
                        "\r\n"
                        "<html><h1>Kek</h1></html>") == 0);
    printf("=== Test 2 success\n");
    map_deinit(&resp->headers);
    free(resp);
}

void test_create_dispatcher() {
    map_init(&url_patterns);
    register_url("/home.html", NULL);
    api_url *func = (api_url *) map_get(&url_patterns, "/home.html");

    assert(strcmp(func->url, "/home.html") == 0);
    //array_list_free_all(url_patterns);
    //free(url_patterns);
    printf("=== Test 3 success\n");
}

void helper_test_home_page(HttpRequest *req, HttpResponse *resp) {
    if (req->method == GET) {
        resp->status_code = 200;
        set_response_header(resp, "cache-control", "no-store");
        set_response_header(resp, "content_type", "text/html");
        strcpy(resp->data, "<html><h1>Hello from server!</h1></html>");
    }
}


void test_process_request() {
    char *test_file_name1 = "../tests/test1.txt";
    int file_fd = open(test_file_name1, O_WRONLY | O_CREAT, 0777);
    Request *r = malloc(sizeof(Request));
    server_init();
    register_url("/home.html", helper_test_home_page);
    *r = (Request) {.id = 0, .request = "GET /home.html HTTP/1.1\r\n\r\n", .client_fd = file_fd};
    int ind = array_list_add(reqs, r);
    void *req = array_list_get(reqs, ind);
    process_request(req);
    file_fd = open(test_file_name1, O_RDONLY);
    char buff[MAX_REQUEST_LENGTH];
    int l = read(file_fd, buff, MAX_REQUEST_LENGTH);
    if (l == -1) {
        printf("Test 4: error in reading; ");
        exit(-1);
    }
    buff[l] = '\0';
    printf("%s \n", buff);
    assert(strcmp(buff, "HTTP/1.1 200\r\n"
                        "Content-length: 42\r\n"
                        "content_type: text/html\r\n"
                        "cache-control: no-store\r\n"
                        "\r\n"
                        "<html><h1>Hello from server!</h1></html>") == 0);
    close(file_fd);
    server_deinit();
    printf("=== Test 4 success\n");
}

void test_congestion() {
    int n = fork();
    if (n == 0) {
        server_init();
        register_url("/home.html", helper_test_home_page);
        server_listen();
        sleep(30);
        server_deinit();
        printf("Server deinited\n");
    } else {
        char buff[MAX_REQUEST_LENGTH];
        int number_of_reqs = 1000;
        char *correct_resp = "HTTP/1.1 200\r\n"
                             "Content-length: 42\r\n"
                             "content_type: text/html\r\n"
                             "cache-control: no-store\r\n"
                             "\r\n"
                             "<html><h1>Hello from server!</h1></html>";
        for (int i = 0; i < number_of_reqs; i++) {

            int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in servaddr;

            memset(&servaddr, 0, sizeof(struct sockaddr_in));
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = inet_addr(LOCALHOST);
            servaddr.sin_port = SERVER_PORT;

            if (connect(socket_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
                printf("Connection failed\n");
            } else {
                write(socket_fd, "GET /home.html HTTP/1.1\r\n\r\n", 100);
                int n = read(socket_fd, buff, MAX_REQUEST_LENGTH);
                buff[n] = '\0';
                assert(strcmp(buff, correct_resp) == 0);
                printf("%d request sent/response got\n", i);
            }
            close(socket_fd);
        }
        printf("=== Test 5 success");
        exit(0);
    }
}

int main() {
    test_parse();
    test_resp_string();
    test_create_dispatcher();
    test_process_request();
    test_congestion();
}