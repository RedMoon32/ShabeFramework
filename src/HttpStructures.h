#define MAX_REQUEST_LENGTH 1000

#ifndef STRUCTURES
#define STRUCTURES

typedef struct {
	char request[1000];
	int clientfd;
	int id;	
} Request;


typedef struct{

} HttpRequest;

typedef struct{

} HttpResonse;

#endif