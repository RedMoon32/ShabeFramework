#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "Dispatcher.h"
#include "HttpStructures.h"
#include "server.h"
#include "Parser.h"

#define PORT 4000
#define MAX_REQUESTS 1000

Request *reqs[MAX_REQUESTS] = {NULL};

int Socket(struct sockaddr_in *address) {

    int server_fd;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);

    memset(address->sin_zero, '\0', sizeof address->sin_zero);


    if (bind(server_fd, address, sizeof(*address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

int append_to_requests(char *buffer, int new_socket, Request **reqs) {
    for (int i = 0; i < MAX_REQUESTS; i++) {
        if (reqs[i] == NULL) {
            reqs[i] = (Request *) malloc(sizeof(Request));
            reqs[i]->clientfd = new_socket;
            reqs[i]->id = i;
            memcpy(reqs[i]->request, buffer, strlen(buffer));
            return i;
        }
    }
    return NULL;
}

void process_request(Request *req) {
    HttpRequest *req_res = parse_str_to_req(req->request);
    api_url_func *processor = dispatch(req_res);
    if (processor == NULL)
        return;
    HttpResponse *resp = malloc(sizeof(HttpResponse));
    resp->headers = malloc(sizeof(map_str_t));
    map_init(resp->headers);
    processor(req_res, resp);
    char result[MAX_REQUEST_LENGTH];
    char clength[10];
    sprintf(clength, "%d", strlen(resp->data) + 2);
    map_set(resp->headers, "Content-length", clength);
    parse_resp_to_str(resp, result);
    write(req->clientfd, result, MAX_REQUEST_LENGTH);

    free(resp->headers);
    free(resp);
    free(req_res->headers);
    free(req_res);
    close(req->clientfd);
    reqs[req->id] = NULL;
    free(req);
}

void server_listen(int server_fd, struct sockaddr *address) {
    int addrlen = sizeof(address);
    int new_socket;
    long valread;
    while (1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, address, (socklen_t *) &addrlen)) < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[MAX_REQUEST_LENGTH] = {0};
        valread = read(new_socket, buffer, MAX_REQUEST_LENGTH);
        int last_req = append_to_requests(buffer, new_socket, reqs);
        printf("%s\n", buffer);
        process_request(reqs[last_req]);

        printf("------------------Response message sent-------------------\n");
        close(new_socket);
    }
}

void start_server() {
    int server_fd;
    struct sockaddr_in address;
    server_fd = Socket(&address);
    server_listen(server_fd, &address);
}