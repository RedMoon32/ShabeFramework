#include <assert.h>
#include "../src/HttpStructures.h"
#include "../src/Parser.h"

void test1(){
	Request r= {.request = "GET\nWhy\nLife\nIs\nSo\nFucking\nHARD\n", .clientfd = 444, .id = 1};
	HttpRequest* ans_req=parse(&r);
	assert(ans_req->method == GET);
	printf("Test2 successfull\n");
}

int main(){
	test1();
}