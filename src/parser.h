#include "http_structures.h"

HttpRequest* parse_str_to_req(char *req);
void parse_resp_to_str(HttpResponse *resp, char *dest);