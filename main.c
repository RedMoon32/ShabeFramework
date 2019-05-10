//
// Created by rinat on 03.05.19.
//

#include <server.h>
#include <Dispatcher.h>
#include <HttpStructures.h>
#include <map.h>

void home_page(HttpRequest *req, HttpResponse *resp) {
    if (req->method == GET) {
        resp->status_code = 200;
        strcpy(resp->data, "<html>"
                           "<h1>"
                           "<a href=\"http://127.0.0.1:7000/login.html\">""Login Here</a>"
                           "<body>HELLO WORLD</br></body>"
                           "</h1><"
                           "/html>");
    } else if (req->method == POST) {

    }
}

void login_page(HttpRequest *req, HttpResponse *resp) {
    resp->status_code = 200;
    strcpy(resp->data, "<html><form>\n"
                       "  First name:<br>\n"
                       "  <input type=\"text\" name=\"firstname\"><br>\n"
                       "  Last name:<br>\n"
                       "  <input type=\"text\" name=\"lastname\">\n"
                       "</form></html>");
}

int main() {
    reg_static_url("/home", "../temp1.html");
    reg_static_url("/login", "../temp2.html");
    //reg_url("/home.html", home_page);
    //reg_url("/login.html", login_page);
    start_server();
    return 0;
}