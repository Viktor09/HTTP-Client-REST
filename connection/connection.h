#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

void error(const char *msg);
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);
void send_to_server(int sockfd, char *message);
char *receive_from_server(int sockfd);
void close_connection(int sockfd);
#endif