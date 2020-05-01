#include <http_server.h>
#include <dispatcher.h>

// Each function accepts two arguments - request and response
void hello_page(HttpRequest *req, HttpResponse *resp) {
    if (req->method == GET) {
        make_response(200, "<html><h1>Hello from server!</h1></html", resp);
    } else if (req->method == POST){
        printf("New data %s has come of type %s \n", req->data, get_request_header(req, CONTENT_TYPE));
        BAD_RESPONSE(resp);
    }
}

int main() {
    // initialize all memory for server
    server_init();
    // set static folder
    set_static_folder("./examples/");
    // register our function in dispatcher
    register_url("/home", hello_page);
    // or register html page from some file
    register_static_url("/login", "./examples/temp2.html");
    // start to listen on 8000 port
    server_listen();
}