#include "HttpStructures.h"
#define DEFAULT_CHILDS_NUM 10

/*
HTTP/1.1 200 OK
Content-Type: application/vnd.api+json

{
  "data": [{
    "type": "articles",
    "id": "1",
    "attributes": {
      "title": "JSON:API paints my bikeshed!",
      "body": "The shortest article. Ever.",
      "created": "2015-05-22T14:56:29.000Z",
      "updated": "2015-05-22T14:56:28.000Z"
    },
    "relationships": {
      "author": {
        "data": {"id": "42", "type": "people"}
      }
    }
  }],
  "included": [
    {
      "type": "people",
      "id": "42",
      "attributes": {
        "name": "John",
        "age": 80,
        "gender": "male"
      }
    }
  ]
}
*/

typedef struct{
    char *name = "";
    int childs_num;
    void *childs[DEFAULT_CHILDS_NUM];
    void *parent;
} JSON_Node;

typedef struct{
	char connection_type[50];
	char server[50];
	int content_length;
	char content_type[50];
	char remote_address[20];
    JSON_Node main;
} JSON_Request;