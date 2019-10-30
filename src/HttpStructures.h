#define MAX_REQUEST_LENGTH 1000

#ifndef STRUCTURES
#define STRUCTURES

typedef struct {
	char request[1000];
	int clientfd;
	int id;	
} Request;


typedef struct{
	char day_name[10];
	int day, mounth, year;
	int sec, min, hour;	
} Date;

typedef struct{
	char connection_type[50];
	char server[50];
	int content_length;
	char content_type[50];
	char remote_address[20];
} HttpRequest;

typedef struct{

} HttpResonse;

#endif