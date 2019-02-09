#include "libs/map.h"

#define MAX_REQUEST_LENGTH 1000	
#define NUMBER_OF_METHODS 8

#ifndef STRUCTURES
#define STRUCTURES

enum Method {GET=0,HEAD=1,POST=2,PUT=3,DELETE=4,CONNECT=5,OPTIONS=6,TRACE=7};
extern char http_methods[8][10];

typedef struct {
	char request[MAX_REQUEST_LENGTH];
	int clientfd;
	int id;	
} Request;


typedef struct{
	enum Method method;
	char *url;
	map_void_t *headers;
	//map_void_t * data; 
} HttpRequest;

typedef struct{

} HttpResponse;

#endif