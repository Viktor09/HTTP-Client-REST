#include "./parson/parson.h"
#include "./buffer/buffer.h"
#include "./connection/connection.h"
#include "./request/request.h"
#include "./helper/helper.h"

int main(void){

    char *cookie = calloc(1000, sizeof(char));
    char *token = calloc(1000, sizeof(char));
    int flag_already_logged = 0; 
    int flag_entered_lib = 0;
   
    for( ; ; ){
        int sockfd_server = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
        char *input_command = calloc(30, sizeof(char));
        scanf("%s", input_command);

        if(strcmp("register", input_command) == 0){
            register_user(sockfd_server);
        } else if(strcmp("login", input_command) == 0){
            login_user(sockfd_server, cookie, token, &flag_already_logged);
        } else if(strcmp("enter_library", input_command) == 0){
            access_library(sockfd_server, cookie, token, flag_already_logged, &flag_entered_lib);
        } else if(strcmp("get_books", input_command) == 0){
            get_books(sockfd_server, cookie, token, flag_already_logged,flag_entered_lib);
        } else if(strcmp("get_book", input_command) == 0){
           get_book_details(sockfd_server, cookie, token, flag_already_logged, flag_entered_lib);
        } else if(strcmp("add_book", input_command) == 0){
            int invalid_data = 0;
            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            add_book_helper(&root_value, &root_object, &invalid_data);
            add_book(sockfd_server, cookie, token, flag_already_logged, root_value, root_object, invalid_data, flag_entered_lib);
        } else if(strcmp("delete_book", input_command) == 0){
            delete_book_from_library(sockfd_server, cookie, token, flag_already_logged, flag_entered_lib);
        } else if(strcmp("logout", input_command) == 0){
            logout_user(sockfd_server, cookie, token, &flag_already_logged, &flag_entered_lib);
	    memset(cookie, 0, 1000);
	    memset(token, 0, 1000);
        } else if( strcmp("exit", input_command) == 0){
            break;
        } else {
            printf("Error: [Invalid command!]\n");
            continue;
        }
        
        close_connection(sockfd_server);
    }

    
    return 0;
}