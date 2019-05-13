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
#include <stdbool.h>

#define HASH_LEN 34	
#define MAX_USER_NAME_LEN 32
#define LICENSE_KEY_LENGTH 20

#define PARENTS_DB_NAME "../web/users.csv"

typedef enum {
	SUCCESS,
	FAILUR
} STATUS;

typedef enum {
	USER_NAME_FIELD = 1,
	PASSWORD_FIELD = 2
} CSV_FIELD;

typedef enum {
	REGISTERED_SUCCESSFULLY,
	USERNAME_TAKEN,
	INVALID_USERNAME,
	PASSWORD_VERIFICTION_INVALID
} registration_status;

typedef struct {
	char licenses_key[LICENSE_KEY_LENGTH];
} licenses_key_item;

registration_status register_new_parent(char username[], char password[]);
STATUS add_parent_to_DB(char username_input[], char password_input[]);
STATUS log_in_parent(char username_input[], char password_input[]);
STATUS licenses_key_validation (char licenes_key_input[]);
int parent_exsist_in_DB(char username_input[]);
void print_all_license_key();
char * get_csv_field(char* line, int num);