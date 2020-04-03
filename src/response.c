#include "response.h"

void make_response(int status_code, char message[DATA_LENGTH], HttpResponse* resp){
    resp->status_code = status_code;
    strcpy(resp->data, message);
}
