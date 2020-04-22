# SHABE 
1.0.1 latest stble version

*"Reinventing wheels is the best thing in the world" (c) C programmer*  
  
Shabe is a new http-server written fully in C

## Contribution
Contribution to this repo (for your experience or just fun) is highly appreciated.  
Write me via telegram - @vionenv

## What's inside
Simple overview of architecture:    
![Architecture](https://github.com/RedMoon32/Shabe/blob/master/ShabeFramework.jpg)

## Documentation

### Installation & Importing

1. Just download this repo and put this folder into your project's root (or anywhere you want) 
2. Then you can include it in your project:  
`#include "Shabe/src/http_server.h`  
`#include "Shabe/src/http_structures.h"`  
`#include "Shabe/src/dispatcher.h`

### Usage

1. First initialize server (memory allocation inside):  
`server_init()`
2. Then you create & register your urls:  
   Simple example:  
   `void hello_page(HttpRequest *req, HttpResponse *resp) {//some_code} `
   `register_url("/home/", hello_page);`  
   or `register_static_url("/login/", "temp2.html");`
3. Run the server (will run on 8000 port):
    `server_listen();`

**Simple example:** 
```c        
// includes are just for my example
#include "Shabe/src/http_server.h"
#include "Shabe/src/dispatcher.h"
#include "Shabe/src/http_structures.h"

// Each function accepts two arguments - request and response
void hello_page(HttpRequest *req, HttpResponse *resp) {
   if (req->method == GET) {
       resp->status_code = 200;
       strcpy(resp->data, "<html><h1>Hello from server!</h1></html");
   } else if (req->method == POST){
       resp->status_code = 200;
       printf("New data %s has come of type %s \n", req->data, get_request_header(req, CONTENT_TYPE));
   }
}

int main() {
   // initialize all memory for server
   server_init();
   // register our function in dispatcher
   register_url("/home/", hello_page);
   // or register html page from some file
   register_static_url("/login/", "temp2.html");
   // start to listen on 8000 port
   server_listen();
}
   ```
*More functions can be found in header files*


<sub><sup>Created and supported by Rinat Babichev as pet-project.<sub><sup>
