#define MAX_REQUEST_LENGTH 1000

#ifndef STRUCTURES
#define STRUCTURES

typedef enum { GET, PUT, POST, DELETE} type;

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
	enum request type;
	char user_agent[1000];
	char connection_type[50];
	char server[50];
	int content_length;
	char content_type[50];
	char remote_address[20];
	struct Date* request_date;
	struct Date* last_modified;
} HttpRequest;

typedef struct{

} HttpResonse;

#endif