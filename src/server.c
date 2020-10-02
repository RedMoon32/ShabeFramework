#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "HttpStructures.h"
#include "server.h"


#define TESTING
#define PORT 8080
#define MAX_REQUESTS 1000

int init(struct sockaddr_in* address){

    int server_fd;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons( PORT );
    
    memset(address->sin_zero, '\0', sizeof address->sin_zero);
    
    
    if (bind(server_fd, address, sizeof(*address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void append_to_requests(char *buffer,int new_socket,Request **reqs){
    for (int i=0;i<MAX_REQUESTS;i++){
            if (reqs[i] == NULL){
                reqs[i] = (Request*) malloc(sizeof(Request));
                reqs[i]->clientfd = new_socket;
                memcpy(reqs[i]->request,buffer,MAX_REQUEST_LENGTH);
                break;
            }
        }
}

void server_listen(int server_fd,struct sockaddr *address){
     int addrlen = sizeof(address);
     int new_socket;
     long valread;
     char *hello = "Hello from server";
     Request* reqs[MAX_REQUESTS];
     while(1)
     {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        append_to_requests(buffer,new_socket,reqs);
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
      }
}

void server_listen_multi(int server_fd,struct sockaddr *address){
     int addrlen = sizeof(address);
     int new_socket;
     long valread;
     char *hello = "Hello from server";
    char buffer[30000] = {0};
     Request* reqs[MAX_REQUESTS];
     while(1)
    {
        new_socket = accept(server_fd, NULL, NULL);
        if(new_socket < 0)
        {
            perror("accept error");
            exit(3);
        }

        switch(fork())
        {
        case -1:
            perror("fork error");
            break;

        case 0:
            close(server_fd);
            while(1)
            {
                valread = recv(new_socket, buffer, 1024, 0);
                if(valread <= 0) break;
                send(new_socket, buffer, valread, 0);
            }

            close(new_socket);
            _exit(0);

        default:
            close(new_socket);
        }
    }

    close(server_fd);
}

#ifndef TESTING

int main(int argc, char const *argv[])
{
    int server_fd;
    struct sockaddr_in address;
    
    
    server_fd = init(&address);
    server_listen(server_fd,&address);

    return 0;
}

#endif