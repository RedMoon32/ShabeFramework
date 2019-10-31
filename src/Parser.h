#include <string.h>
#include "HttpStructures.h"

HttpRequest parse_http_request(char *http_request);
int line_elements_count(char *http_request, int start);
Date parse_date_str(char *date);
void first_line_handling(struct HttpRequest* new_request);