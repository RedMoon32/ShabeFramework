#include "Parser.h"

int line_elements_count(char *http_request, int start){
    for(int i = start; 1; i++){
        if(http_request[i] != '\n'){
            return i - start;
        }
    }
}

void first_line_handling(struct HttpRequest* new_request, char* line){
    int i = 0;
    if(line[0] == 'G') new_request->type = GET;
    else if(line[0] == 'D') new_request->type = DELETE;
    else if(line[0] == 'P' && line[0] == 'U') new_request->type = PUT;
    else new_request->type = POST;
    while (line[i] != ' ')
        i++;
    int temp_i = i + 1; i++;
    while (line[i] != ' ')
        i++;
    memcpy(new_request->domen, line, i - temp_i);
}

HttpRequest* parse_http_request(char *http_request){
    struct HttpRequest *new;
    char* line;
    int i = 0, line_cnt = 0;
    // Reading whole string
    while(http_request[i] != '\0'){
        int temp_i = i;
        i += line_elements_count(http_request, i);
        // Dividing string to the lines
        line = (char*)malloc(sizeof(char)*(i - temp_i));
        switch (line_cnt)
        {
        case 0:
            first_line_handling(new, line);
            break;
        
        default:
            break;
        }
        line_cnt++;
    }
}