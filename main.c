//
// Created by rinat on 03.05.19.
//

#include <server.h>
#include <dispatcher.h>
#include <http_structures.h>
#include <map.h>

int main() {
    server_init();
    register_static_url("/home", "../temp1.html");
    register_static_url("/login", "../temp2.html");
    //register_url("/home.html", home_page);
    //register_url("/login.html", login_page);
    server_listen();
    return 0;
}