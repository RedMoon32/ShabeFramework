#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "HttpStructures.h"
#include "Parser.h"


HttpRequest* parse(Request* req){
	
	char *line;
	char *delimiters = "\n";
	char *buffer = malloc(sizeof(req->request));
	memcpy(buffer,req->request,MAX_REQUEST_LENGTH);
	HttpRequest* ans_req = (HttpRequest*)malloc(sizeof(HttpRequest));

	line = strtok(buffer,"\n");
	if (line == NULL)
		return NULL;

	char* lines[100];
	int i = 0;
	
	while (line != NULL){
		lines[i] = line;
		line = strtok(NULL,"\n");
		i++;
	}

	char* first_line = lines[0];
	char* word = strtok(first_line," ");
	
	for (int i=0;i<NUMBER_OF_METHODS;i++){
		if (strcasecmp(word,http_methods[i])==0){
			ans_req->method = i;		
			break;
		}
	}
	return ans_req;
}