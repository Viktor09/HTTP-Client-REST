#include "../buffer/buffer.h"
#include "../connection/connection.h"
#include "request.h"

char *compute_post_request(char *host, char *url, char* content_type, int content_length, char *payload, char *cookies, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));
    
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if(token != NULL){
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line); 
    }
    if (cookies != NULL) {
        sprintf(line, "Cookie: %s", cookies);
        compute_message(message, line);  
    }

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %d\r\n", content_length);
    compute_message(message, line);
   
    sprintf(line, "%s", payload);
    compute_message(message, line);

    memset(line, 0, LINELEN);
    strcat(message, body_data_buffer);

    free(line);
    return message;
}


char *compute_delete_request(char *host, char *url,  char *cookies, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));
    
    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if(token != NULL){
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line); 
    }

    if (cookies != NULL) {
        sprintf(line, "Cookie: %s\r\n", cookies);
        compute_message(message, line);  
    }

    memset(line, 0, LINELEN);
    strcat(message, body_data_buffer);

    free(line);
    return message;
}


char *compute_get_request(char *host, char *url, char *cookie, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);    

    if (cookie != NULL) {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);  
    }

    if(token != NULL){
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line); 
    }

    compute_message(message, "");
    return message;
}