#include "../buffer/buffer.h"
#include "../connection/connection.h"
#include "../request/request.h"
#include "helper.h"

char* extract_cookie(const char* headers) {
    const char* set_cookie = "Set-Cookie: ";
    const char* semicolon = ";";

    char* cookie_start = strstr(headers, set_cookie);
    if (cookie_start == NULL) {
       
        return NULL;
    }

    cookie_start += strlen(set_cookie);

    char* cookie_end = strstr(cookie_start, semicolon);
    if (cookie_end == NULL) {
    
        return NULL;
    }

    int cookie_length = cookie_end - cookie_start;
    char* cookie = malloc(cookie_length + 1);

    strncpy(cookie, cookie_start, cookie_length);
    cookie[cookie_length] = '\0'; 

    return cookie;
}

char* extract_jwt_token_from_json(const char* response_body) {
    const char* token_key = "\"token\":\"";
    char* token_start = strstr(response_body, token_key);
    if (token_start == NULL) {
        return NULL;
    }

    token_start += strlen(token_key);
    char* token_end = strchr(token_start, '"');
    if (token_end == NULL) {
        return NULL;
    }
    int token_length = token_end - token_start;

    char* token = malloc(token_length + 1);
 
    strncpy(token, token_start, token_length);
    token[token_length] = '\0'; 

    return token;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char* extract_json_response(char *response, int ok) {
    char a,b;
    if(ok == 0){
        a = '[';
        b = ']';
    } else if(ok == 1){
        a = '{';
        b = '}';
    }
    char *json_start = strchr(response, a);
    if (json_start == NULL) {
        return NULL; 
    }

    char *json_end = strrchr(response, b);
    if (json_end == NULL) {
        return NULL; 
    }

    int json_length = json_end - json_start + 1;
    char *json_response = (char*)malloc(json_length + 1);
  

    strncpy(json_response, json_start, json_length);
    json_response[json_length] = '\0';

    return json_response;
}
JSON_Value *create_json_user_object(int *valid_register) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    char string[100];
    
    printf("username=");
    scanf(" %99[^\n]", string);
    int i = 0;
    while(i < strlen(string)){
        if(string[i] == ' '){
            *valid_register = 1;
            break;
        }
        i++;
    }
    if(*valid_register == 0)
        json_object_set_string(root_object, "username", string);
    clear_input_buffer();

    printf("password=");
    scanf(" %99[^\n]", string);
    if(*valid_register == 0)
        json_object_set_string(root_object, "password", string);
    clear_input_buffer();

    return root_value;
}

void register_user(int sockfd_server) {
    int valid_register = 0;
    JSON_Value *root_value = create_json_user_object(&valid_register);
    char *json_str = json_serialize_to_string(root_value);

    if(valid_register == 0){
        
        char *message = compute_post_request("34.246.184.49","/api/v1/tema/auth/register", "application/json", strlen(json_str), json_str, NULL, NULL);
        send_to_server(sockfd_server, message);
        char *raspuns = receive_from_server(sockfd_server);

        char *code = raspuns + 9;
        code[4] = '\0';

        if(atoi(code) == 201){
            printf("Succes: [User created!]\n");
        } else{
            printf("Error: [User already registered!]\n");
        }
    } else{
        printf("Error: [User name invalid!]\n");
    } 
}

void login_user(int sockfd_server, char *cookie, char *token, int *flag_already_logged) {
    int valid_register = 0;
    JSON_Value *root_value = create_json_user_object(&valid_register);
   

    if(*flag_already_logged == 1){
        printf("Error: [User is already logged!]\n");
        return;
    }

    if(valid_register == 0){
        *flag_already_logged = 1;
        char *json_str = json_serialize_to_string(root_value);
        char *message = compute_post_request("34.246.184.49","/api/v1/tema/auth/login", "application/json", strlen(json_str), json_str, cookie, token);

        send_to_server(sockfd_server, message);
        char *raspuns = receive_from_server(sockfd_server);

        if(extract_cookie(raspuns) == NULL){
            printf("Error: [User failed to login!]\n");
            *flag_already_logged = 0;
        } else{
            strcpy(cookie, extract_cookie(raspuns));
            printf("Succes: [User succesfully logged in!]\n");
            *flag_already_logged = 1;
        }
    } else{
        *flag_already_logged = 0;
        printf("Error: [User name invalid!]\n");
    }
}

void access_library(int sockfd_server, char *cookie, char *token, int flag_already_logged, int *flag_entered_lib) {
    if(flag_already_logged == 0){
        printf("Error: [User is not logged in!]\n");
        return;
    }
    char *message = compute_get_request("34.246.184.49", "/api/v1/tema/library/access", cookie, token);
    
    send_to_server(sockfd_server, message);
    char *raspuns = receive_from_server(sockfd_server);

    if(extract_jwt_token_from_json(raspuns) == NULL){
        printf("Error: [User failed to get access to library!]\n");
    } else{
        strcpy(token, extract_jwt_token_from_json(raspuns));
        *flag_entered_lib = 1;
        printf("Succes: [User entered the library!]\n");
    }
}
void get_books(int sockfd_server, char *cookie, char *token, int flag_already_logged,int flag_entered_lib) {
    if(flag_already_logged == 0){
        printf("Error: [User is not logged in!]\n");
        return;
    }
    if(flag_entered_lib == 0){
        printf("Error: [User failed to get access to library!]\n");
        return;
    }
    char *message = compute_get_request("34.246.184.49", "/api/v1/tema/library/books", cookie, token);
    send_to_server(sockfd_server, message);
    char *raspuns = receive_from_server(sockfd_server);
    if(extract_json_response(raspuns, 0) == NULL){
        printf("Retrieved book IDs + titles:\n[]\n[]\n");
        return;
    }
    printf("Retrieved book IDs + titles:\n%s\n", extract_json_response(raspuns, 0));
}

void get_book_details(int sockfd_server, char *cookie, char *token, int flag_already_logged, int flag_entered_lib) {
    char *id = calloc(100, sizeof(char));
    printf("id=");
    scanf("%s", id);
    if(atoi(id) == 0){
        printf("Error [Invalid ID of book!]\n");
        return;
    }
    if(flag_already_logged == 0){
        printf("Error: [User is not logged in!]\n");
        return;
    }
    if(flag_entered_lib == 0){
        printf("Error: [User failed to get access to library!]\n");
        return;
    }
    char *url = calloc(1000, sizeof(char));
    strcat(url, "/api/v1/tema/library/books/");
    strcat(url, id);
    char *message = compute_get_request("34.246.184.49", url, cookie, token);
    send_to_server(sockfd_server, message);
    char *raspuns = receive_from_server(sockfd_server);
    printf("%s\n", extract_json_response(raspuns, 1));
}

void add_book_helper(JSON_Value **root_value, JSON_Object **root_object, int *invalid_data) {

    *root_value = json_value_init_object();
    *root_object = json_value_get_object(*root_value);

    char str[100];
    
    printf("title=");
    if(scanf(" %99[^\n]", str) != 1 ){
            *invalid_data = 1;
    }
    json_object_set_string(*root_object, "title", str);
    clear_input_buffer();
    
    printf("author=");
    if(scanf("%99[^\n]", str) != 1 ){
            *invalid_data = 1;
    }
    json_object_set_string(*root_object, "author", str);
    clear_input_buffer();

    printf("publisher=");
    if(scanf("%99[^\n]", str) != 1 ){
            *invalid_data = 1;
    }
    json_object_set_string(*root_object, "publisher", str);
    clear_input_buffer();

    printf("genre=");
    if(scanf("%99[^\n]", str) != 1 ){
            *invalid_data = 1;
    }
    json_object_set_string(*root_object, "genre", str);
    clear_input_buffer();

    printf("page_count=");
    if(scanf("%99[^\n]", str) != 1 ){
            *invalid_data = 1;
    }
    if(atoi(str) == 0){
        *invalid_data = 1;
    }
    json_object_set_string(*root_object, "page_count", str);
    clear_input_buffer();


    return ;
}

void add_book(int sockfd_server, char *cookie, char *token, int flag_already_logged, JSON_Value *root_value, JSON_Object *root_object, int invalid_data, int flag_entered_lib) {
    if (flag_already_logged == 0) {
        printf("Error: [User is not logged in!]\n");
        return;
    }
    if (flag_entered_lib == 0) {
        printf("Error: [User failed to get access to library!]\n");
        return;
    }

    if (invalid_data == 0) {
        char *json_str = json_serialize_to_string(root_value);

        char *message = compute_post_request("34.246.184.49", "/api/v1/tema/library/books", "application/json", strlen(json_str), json_str, cookie, token);
        send_to_server(sockfd_server, message);
        char *raspuns = receive_from_server(sockfd_server);

        char *code = raspuns + 9;
        code[4] = '\0';
        if (atoi(code) == 200) {
            printf("Succes: [User added the book correctly!]\n");
        } else {
            printf("Error: [User didn't add the book!]\n");
        }

        json_free_serialized_string(json_str);
    } else { 
        printf("Error: [User entered invalid fields]\n");
    }

}

void delete_book_from_library(int sockfd_server, char *cookie, char *token, int flag_already_logged, int flag_entered_lib) {
    if (flag_already_logged == 0) {
        printf("Error: [User is not logged in!]\n");
        return;
    }
    if (flag_entered_lib == 0){
        printf("Error: [User didn't enter in library]");
        return;
    }

    char *id = calloc(100, sizeof(char));
    printf("id=");
    scanf("%s", id);

    if (atoi(id) == 0) {
        printf("Error: [Invalid ID provided!]\n");
        return;
    }

    char *url = calloc(1000, sizeof(char));
    strcat(url, "/api/v1/tema/library/books/");
    strcat(url, id);

    char *message = compute_delete_request("34.246.184.49", url, cookie, token);

    send_to_server(sockfd_server, message);
    char *raspuns = receive_from_server(sockfd_server);

    char *code = raspuns + 9;
    code[4] = '\0';

    if (atoi(code) == 200) {
        printf("Succes: [Book has been successfully deleted!]\n");
    } else {
        printf("Error: [Couldn't delete the book!]\n");
    }

}

void logout_user(int sockfd_server, char *cookie, char *token, int *flag_already_logged, int *flag_entered_lib) {
    if (*flag_already_logged == 0) {
        printf("Error: [User already logged out or doesn't exist!]\n");
        return;
    }

    char *message = compute_get_request("34.246.184.49", "/api/v1/tema/auth/logout", cookie, token);
    send_to_server(sockfd_server, message);
    char *raspuns = receive_from_server(sockfd_server);

    char *code = raspuns + 9;
    code[4] = '\0';
    
    if (atoi(code) == 200) {
        printf("Succes: [User logged out!]\n");
        *flag_entered_lib = 0;
        *flag_already_logged = 0;
    } else {
        printf("Error: [User couldn't log out!]\n");
    }
}