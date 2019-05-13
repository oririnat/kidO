#include <stdio.h> 

#include <stdlib.h>
#include <string.h>

#define AES_KEY_LEN 16
#define HASH_LEN 34
#define MAX_USER_NAME_LEN 32

typedef struct client_node {
	int socket_fd;
	char name[MAX_USER_NAME_LEN];	
	char encryption_key[AES_KEY_LEN + 1];
	struct client_node * next_client;
} client, * client_ptr;

// global list
client_ptr clients;
void print_clients_list(client_ptr head);
void insert_client(int socket_fd, char name[], client_ptr * head);
void remove_client(int socket_id, client_ptr * head);
