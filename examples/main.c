#include <http_server.h>
#include <dispatcher.h>
#include <http_structures.h>
#include <map.h>

// Each function accepts two arguments - request and response
void hello_page(HttpRequest *req, HttpResponse *resp) {
    if (req->method == GET) {
        resp->status_code = 200;
        strcpy(resp->data, "<html><h1>Hello from server!</h1></html");
    } else if (req->method == POST){
        resp->status_code = 200;
        printf("New data %s has come of type %s \n", req->data, get_request_header(req, CONTENT_TYPE));
    }
}

int main() {
    // initialize all memory for server
    server_init();
    // register our function in dispatcher
    register_url("/home/", hello_page);
    // or register html page from some file
    register_static_url("/login/", "temp2.html");
    // start to listen on 8000 port
    server_listen();
}