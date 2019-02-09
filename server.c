#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "server.h"

#define PORT 8080

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

void server_listen(int server_fd,struct sockaddr *address){
     int addrlen = sizeof(address);
     int new_socket;
     long valread;
     char *hello = "Hello from server";
        
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
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
      }
}

int main(int argc, char const *argv[])
{
    int server_fd;
    struct sockaddr_in address;
    
    
    server_fd = init(&address);
    server_listen(server_fd,&address);

    return 0;
}