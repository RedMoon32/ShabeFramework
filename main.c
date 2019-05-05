//
// Created by rinat on 03.05.19.
//

#include <server.h>
#include <Dispatcher.h>
#include <HttpStructures.h>
#include <map.h>

void home_page(HttpRequest *req, HttpResponse *resp) {
    resp->status_code = 200;
    strcpy(resp->data, "<html><h1>HELLO WORLD</h1></html>");
}

int main() {
    reg_url("/home.html", home_page);
    start_server();
    return 0;
}