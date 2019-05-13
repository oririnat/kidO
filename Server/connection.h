#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <time.h>
#include <stdbool.h>
#include "md5.h"
#include "client_data_structure.h"

#define MTU 1024
#define PORT 50008
#define MAX_VICTIMS_PER_ATTACKER 10
#define MAX_PAYLOAD_TO_BUFFER_COMMEND_LEN 100

#define RSA_BASE64_AES_KEY_SIZE 500
#define ENCRYPTED_RECEIVED_DATA_NAME ".temp_enc_file"
#define ENCRYPTED_TEXT_LEN(len) (int) ((len * 1.36) + 100)

#define HASH_LEN 34
#define MAX_ACTION_LEN 8
#define MAX_USER_NAME_LEN 32
#define MAX_PASSWORD_LEN 16
#define MAX_FORBIDDEN_WORD_LEN 40
#define TIME_AND_DATE_LEN 25

struct sockaddr_in address;

typedef enum {
	LOG_IN,
	REGISTER,
	KEY_EXCHANGE,
	GET_NEW_FORBIDDEN_EVENT,
} action_type;

typedef struct {
	char username[MAX_USER_NAME_LEN];
	char password[MAX_PASSWORD_LEN];
} log_in_protocol;

typedef struct {
	char username[ENCRYPTED_TEXT_LEN(MAX_USER_NAME_LEN)];
	char password[ENCRYPTED_TEXT_LEN(MAX_PASSWORD_LEN)];
} encrypted_log_in_protocol;

typedef union{
	log_in_protocol login;
	log_in_protocol register_parent;
	char action[MAX_ACTION_LEN];
	char forbidden_event_details[TIME_AND_DATE_LEN + MAX_ACTION_LEN + MAX_FORBIDDEN_WORD_LEN + 3];
} main_data;

typedef union{
	encrypted_log_in_protocol encrypted_login;
	encrypted_log_in_protocol encrypted_register_parent;
	char action[ENCRYPTED_TEXT_LEN(MAX_ACTION_LEN)];
	char encrypted_forbidden_event_details[ENCRYPTED_TEXT_LEN(TIME_AND_DATE_LEN + MAX_ACTION_LEN + MAX_FORBIDDEN_WORD_LEN + 3)];
	char key_exchange_buffer[RSA_BASE64_AES_KEY_SIZE];
} encrypted_main_data;

typedef struct {
	action_type action;
	encrypted_main_data data;
} encrypted_general_message_protocol;

int master_socket, addrlen, new_socket, activity, socket_descriptor;
int max_socket_descriptor; 
fd_set readfds; //set of socket descriptors

void initialize_connection();
void add_child_sockets_to_set(client_ptr * client_list);
void recv_parent_aes_key(client_ptr * curr_client, const char * rsa_base64_to_decrypt);
char * encrypt_text (char * text_to_encrypt, char * encrypt_key);
char * decrypt_text (char * text_to_decrypt, char * decryption_key);
