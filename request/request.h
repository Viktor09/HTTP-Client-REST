#ifndef REQUEST_H
#define REQUEST_H

char *compute_post_request(char *host, char *url, char* content_type, int content_length, char *payload, char *cookies, char *token);
char *compute_delete_request(char *host, char *url,  char *cookies, char *token);
char *compute_get_request(char *host, char *url, char *cookie, char *token);

#endif
