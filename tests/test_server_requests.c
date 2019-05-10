#include <assert.h>
#include "../src/server.h"
#include "http_structures.h"

#define MAX_REQUSTS 10

void test1(){
	char* buf = "Hello";
	Request* reqs[10] = {NULL};
	append_to_requests(buf,123,reqs);
	append_to_requests(buf,245,reqs);
	append_to_requests(buf,444,reqs);
	assert (reqs[0]->clientfd == 123);
	assert (reqs[1]->clientfd == 245);
	assert (reqs[2]->clientfd == 444);

	printf(" Test1 Successful\n");
}


int main(){
	test1();
}