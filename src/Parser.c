#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "HttpStructures.h"
#include "Parser.h"


HttpRequest* parse(Request* req){
	
	char *line;
	char *delimiters = "\n";
	char *buffer = malloc(strlen(req->request));
	memcpy(buffer,req->request,strlen(req->request));
	HttpRequest* ans_req = malloc(sizeof(HttpRequest));

	line = strtok(buffer,"\n");

	if (line == NULL){
		return NULL;
	}
	
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

	if (strcasecmp(word,"HTTP/1.1")!=0){
		return NULL;
	}	
	
	return ans_req;
}