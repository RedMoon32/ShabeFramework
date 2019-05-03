#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/HttpStructures.h"
#include "../src/Parser.h"

void test1(){
	Request r = {.request = "GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1", .clientfd = 444, .id = 1};
	HttpRequest* ans_req = parse(&r);

	assert(ans_req != NULL);
	assert(ans_req->method == GET);
	assert(strcmp(ans_req->url,"http://www.w3.org/pub/WWW/TheProject.html")==0);
	
	printf("Test2 successfull\n");
}

void test2(){
	Request r = {.request = "GET /favicon.ico HTTP/1.1\nHost: localhost:8080\nConnection: keep-alive\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36 OPR/52.0.2871.97\nAccept: image/webp,image/apng,image/*,*/*;q=0.8",.clientfd = 44,.id = 1};
	HttpRequest* ans_req = parse(&r);
	assert(strcmp(*map_get(ans_req->headers,"Host"),"localhost:8080")==0);
	assert(strcmp(*map_get(ans_req->headers,"Connection"),"keep-alive")==0);
	//assert(strcmp(*map_get(ans_req->headers,"User-Agent")," Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36 OPR/52.0.2871.97")==0);
}

void test3(){
    
}

int main(){
	//test1();
	test2();
}