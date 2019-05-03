#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "HttpStructures.h"

int init(struct sockaddr_in* address);
void append_to_requests(char *buffer,int new_socket,Request **reqs);
void server_listen(int server_fd,struct sockaddr *address);
void start_server();