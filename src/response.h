#include <dispatcher.h>
#include <http_structures.h>

void make_response(int status_code, char message[DATA_LENGTH], HttpResponse* resp);