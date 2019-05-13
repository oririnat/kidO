#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include "md5.h"

//#define USER_MESSAGE_PROMPT

#define PORT 50008
#define SERVER_IP "127.0.0.1"

#define RSA_BASE64_AES_KEY_SIZE 500
#define MTU 1024 // maximum transformation unit

#define TEMP_RCVE_AES_KEY_FILE_NAME ".temp_aes_key.txt"
#define ENCRYPTED_RECEIVED_DATA_NAME ".temp_enc_file"
#define ENCRYPTED_TEXT_LEN(len) (int) ((len * 1.36) + 100)
#define AES_KEY_LEN 16

#define HASH_LEN 34
#define MAX_ACTION_LEN 8
#define MAX_PASSWORD_LEN 16
#define MAX_USER_NAME_LEN 32
#define MAX_FORBIDDEN_WORD_LEN 40
#define TIME_AND_DATE_LEN 25


int K_S_socket; // global value
char AES_KEY[AES_KEY_LEN + 1];

typedef enum {
	SUCCESS,
	FAILUR
} STATUS;

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
	main_data data;
} general_message_protocol;

typedef struct {
	action_type action;
	encrypted_main_data data;
} encrypted_general_message_protocol;

STATUS create_connection();
void key_exchange (int K_S_socket);
//void log_in_victim();
void K_2_S_encrypted_message_handler(main_data data, action_type action);
//void send_file(char * file_name, action_type action, bool base64);
//void send_keystrock(char * key);
void secure_key_exchange();
char * encrypt_text (char * text_to_encrypt, char * encrypt_key);
char * decrypt_text (char * text_to_decrypt, char * decryption_key);
char * generate_key(char * key_template);