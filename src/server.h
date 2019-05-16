#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "http_structures.h"

extern p_array_list reqs;

void process_request(Request *req);
void server_listen();
void server_init();
void server_deinit();