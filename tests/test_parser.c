#include <assert.h>
#include <stdlib.h>
#include "../src/HttpStructures.h"
#include "../src/Parser.h"

void test1(){
	Request r= {.request = "GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1", .clientfd = 444, .id = 1};
	HttpRequest* ans_req=parse(&r);

	assert(ans_req!=NULL);
	assert(ans_req->method == GET);
	assert(strcmp(ans_req->url,"http://www.w3.org/pub/WWW/TheProject.html")==0);
	
	printf("Test2 successfull\n");
}

int main(){
	test1();
}