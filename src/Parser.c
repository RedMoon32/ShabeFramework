#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "libs/map.h"
#include "HttpStructures.h"
#include "Parser.h"


HttpRequest* parse(Request* req){
	
	char *line;
	char *delimiters = "\n";
	char *buffer = malloc(strlen(req->request));
	memcpy(buffer,req->request,strlen(req->request));
	HttpRequest* ans_req = malloc(sizeof(HttpRequest));

	line = strtok(buffer,"\n");

	char* lines[100] = {NULL};
	int i = 0;

	while (line != NULL){
		lines[i] = line;
		line = strtok(NULL,"\n");
		i++;
	}

	char* first_line = lines[0];
	char* word = strtok(first_line," ");

	int found = 0;
	for (int i=0;i<NUMBER_OF_METHODS;i++){
		if (strcasecmp(word,http_methods[i])==0){
			ans_req->method = i;		
			found = 1;
			break;	
		}
	}

	if (!found){
		return NULL;
	}

	word = strtok(NULL," ");
	ans_req->url =malloc(strlen(word));
	memcpy(ans_req->url,word,strlen(word));
	word = strtok(NULL," ");
	i = 1;
	
	if (strcasecmp(word,"HTTP/1.1")!=0){
		return NULL;
	}

	map_void_t* headers = malloc(sizeof(map_void_t));
	map_init(headers);
	ans_req->headers = headers;

	char *header,*value;

	while (lines[i]!=NULL){
		header = strtok(lines[i]," :");
		value = strtok(NULL," ");
		char *vbuffer = malloc(strlen(value));
		memcpy(vbuffer,value,strlen(value));
		map_set(headers,header,vbuffer);
		i++;
	}	

	return ans_req;
}