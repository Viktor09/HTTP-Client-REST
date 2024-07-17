#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parson/parson.h"

char* extract_cookie(const char* headers);
char* extract_jwt_token_from_json(const char* response_body);
void clear_input_buffer();
char* extract_json_response(char *response, int ok);
JSON_Value* create_json_user_object(int *valid_register);
void register_user(int sockfd_server);
void login_user(int sockfd_server, char *cookie, char *token, int *flag_already_logged);
void access_library(int sockfd_server, char *cookie, char *token, int flag_already_logged, int *flag_entered_lib);
void get_books(int sockfd_server, char *cookie, char *token, int flag_already_logged, int flag_entered_lib);
void get_book_details(int sockfd_server, char *cookie, char *token, int flag_already_logged, int flag_entered_lib);
void add_book_helper(JSON_Value **root_value, JSON_Object **root_object, int *invalid_data);
void add_book(int sockfd_server, char *cookie, char *token, int flag_already_logged, JSON_Value *root_value, JSON_Object *root_object, int invalid_data, int flag_entered_lib);
void delete_book_from_library(int sockfd_server, char *cookie, char *token, int flag_already_logged, int flag_entered_lib);
void logout_user(int sockfd_server, char *cookie, char *token, int *flag_already_logged, int *flag_entered_lib);

#endif