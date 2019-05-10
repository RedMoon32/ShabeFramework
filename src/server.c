#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "dspatcher.h"
#include "http_structures.h"
#include "server.h"
#include "parser.h"

#define PORT 8000
#define MAX_REQUESTS 1000

Request *reqs[MAX_REQUESTS] = {NULL};

/** Initialize master tcp - socket on predefined port in IPV4 space
 *
 * @return file descriptor of new socket (or will exit if error happened)
 */
int Socket() {

    struct sockaddr_in address;
    int server_fd;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);


    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

/** Add new request to the queue of incoming requests
 *
 * @param buffer - data received
 * @param new_socket - socket of pending connection
 * @param reqs - array of pointers to request struct
 * @return NULL on error, index of new request in queue on success
 */
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

/** Function which processes http request:
 * Parses http request -> perform action based on registered urls ->
 * converts http response to string and returns it to client
 * @param req - info about new req(client_fd, req_id, raw_request)
 */
void process_request(Request *req) {
    HttpRequest *req_res = parse_str_to_req(req->request);
    if (req_res == NULL)
        goto out;
    api_url_func *processor = get_request_processor(req_res);

    HttpResponse *resp = malloc(sizeof(HttpResponse));
    resp->headers = malloc(sizeof(map_str_t));
    map_init(resp->headers);
    if (processor == NULL) {
        resp->status_code = 404;
    } else {
        processor(req_res, resp);
    }
    char result[MAX_REQUEST_LENGTH];
    char clength[10];
    sprintf(clength, "%d", strlen(resp->data) + 2);
    map_set(resp->headers, "Content-length", clength);
    parse_resp_to_str(resp, result);
    write(req->clientfd, result, MAX_REQUEST_LENGTH);
    printf("<- %s %s %d\n", http_methods[req_res->method], req_res->url, resp->status_code);
    free(resp->headers);
    free(resp);
    free(req_res->headers);
    free(req_res);

    out:
    close(req->clientfd);
    reqs[req->id] = NULL;
    free(req);

}

/**
 * Main listening part
 * @param server_fd - master socket fd
 */
void server_listen(int server_fd) {
    struct sockaddr address;
    int addrlen = sizeof(address);
    int new_socket;
    int valread;
    printf("==========<Server is listening on port 8000>==========\n");
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[MAX_REQUEST_LENGTH] = {0};
        valread = read(new_socket, buffer, MAX_REQUEST_LENGTH);
        if (valread == 0 || valread == -1) {
            close(new_socket);
            continue;
        }
        int last_req = append_to_requests(buffer, new_socket, reqs);
        process_request(reqs[last_req]);
    }
}

/** Function to start server on 8000 port
 *
 */
void start_server() {
    int server_fd;
    server_fd = Socket();
    server_listen(server_fd);
}