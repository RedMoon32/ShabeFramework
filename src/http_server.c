#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>

#include "dispatcher.h"
#include "http_structures.h"
#include "http_server.h"
#include "parser.h"
#include <alist.h>
#include <pthread.h>


#define NOT_FOUND_STRING "<html><h4> 404 not found </h4></html>"


p_array_list reqs;
int master_fd;
int SERVER_PORT;
int listening = 0;

/** Initialize master tcp socket on predefined port in IPV4 space
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
    address.sin_port = htons(SERVER_PORT);

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
int append_to_requests(char *buffer, int new_socket) {
    Request *new_req = (Request *) malloc(sizeof(Request));
    new_req->client_fd = new_socket;
    memcpy(new_req->request, buffer, strlen(buffer));
    int ind = array_list_add(reqs, new_req);
    new_req->id = ind;
    return ind;
}

/** Function which processes http request:
 * Parses http request -> perform action based on registered urls ->
 * converts http response to string and returns it to client
 * @param req - info about new req(client_fd, req_id, raw_request)
 */
void process_request(Request *req) {
    HttpRequest *req_res = parse_str_to_req(req->request);
    if (!get_request_header(req_res, CONTENT_TYPE)) {
        map_set(&req_res->headers, CONTENT_TYPE, "text/plain");
    }
    if (req_res == NULL)
        goto out;
    api_url_func *processor = get_request_processor(req_res);

    HttpResponse *resp = malloc(sizeof(HttpResponse));

    map_init(&resp->headers);
    if (processor == NULL) {
        resp->status_code = 404;
        strcpy(resp->data, NOT_FOUND_STRING);
    } else {
        processor(req_res, resp);
    }
    char result[MAX_REQUEST_LENGTH];
    char clength[10];
    sprintf(clength, "%d", strlen(resp->data) + 2);
    map_set(&resp->headers, "Content-length", clength);
    parse_resp_to_str(resp, result);
    strcat(result, "\0");
    int st = write(req->client_fd, result, strlen(result) + 1);
#ifdef TESTING
    printf("Response %s:\n",result);
#endif
    printf("<- %s %s %d\n", http_methods[req_res->method], req_res->url, resp->status_code);

    map_deinit(&resp->headers);
    free(resp);
    map_deinit(&req_res->headers);
    free(req_res);

    out:
    st = close(req->client_fd);
    array_list_remove_at(reqs, req->id);
    free(req);

}


/**
 * Main listening part
 * @param server_fd - master socket fd (with some port binded to this socket)
 */
void *server_listen_() {
    int server_fd = master_fd;
    struct sockaddr address;
    int addrlen = sizeof(address);
    int new_socket;
    int valread;

    printf("\n* Server is listening on port 8000\n* Enter 'q' to exit\n\n");

    while (listening) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("In accept");
        }

        char buffer[MAX_REQUEST_LENGTH] = {0};
        valread = read(new_socket, buffer, MAX_REQUEST_LENGTH);
        if (valread == 0 || valread == -1) {
            close(new_socket);
            continue;
        }
#ifdef TESTING
        printf("%s - new request\n", buffer);
#endif
        int last_req = append_to_requests(buffer, new_socket);
        process_request(array_list_get(reqs, last_req));
    }
}

void server_deinit() {
    listening = 0;
    const char *key;
    map_iter_t iter = map_iter(&url_patterns);

    while ((key = map_next(&url_patterns, &iter))) {
        free(*map_get(&url_patterns, key));
    }

    array_list_free_all(reqs);
    free(reqs);

    map_deinit(&url_patterns);
    close(master_fd);
}

/**
 * Ask for out and if 'q' symbol then exit
 */
void *ask_out() {
    while (1) {
        char c;
        scanf("%c", &c);
        if (c == 'q') {
            printf("Closing server ...\n");
            server_deinit();
            exit(0);
        }

    }
}

/**
 * Function to start listening in new thread
 */
void server_listen() {
    listening = 1;
    pthread_t listen_thread, ask_out_thread;
    pthread_create(&listen_thread, NULL, server_listen_, NULL);
    pthread_create(&ask_out_thread, NULL, ask_out, NULL);
    pthread_join(listen_thread, NULL);
}

/**
 * Function to start server on 8000 port
 */
void server_init() {
    SERVER_PORT = 8000;
    reqs = create_array_list(100);
    map_init(&url_patterns);
    master_fd = Socket();
}