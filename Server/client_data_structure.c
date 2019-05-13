#include "client_data_structure.h"

//display the list
void print_clients_list(client_ptr head) {
	client_ptr client = head;
	printf("\n[ ");
	
	//start from the beginning
	while(client != NULL) {
		printf("(socket_fd : %d, name : %s), \n", client->socket_fd, client->name);
		client = client->next_client;
	}
	printf(" ]\n");
}

//insert link at the first location
void insert_client(int socket_fd, char name[], client_ptr * head) {
	//create a new client
	client_ptr new_client = (client_ptr) malloc(sizeof(client));
	
	//set info
	new_client->socket_fd = socket_fd;
	strcpy(new_client->name, name);
	
	//point it to old first node
	new_client->next_client = *head;
	
	//point first to new first node
	*head = new_client;
}

void remove_client(int socket_id, client_ptr * head){
	//start from the first link
	client_ptr curr_client = * head;
	client_ptr prev_client = NULL;
	
	//if list is empty
	if(head == NULL) 
		return;

	// navigate through list
	while(curr_client->socket_fd != socket_id) {
		//if it is last node
		if(curr_client->next_client == NULL) 
			return;
		else {
			//store reference to current link
			prev_client = curr_client;
			//move to next link
			curr_client = curr_client->next_client;
		}
	}
	//found a match, update the link
	if(curr_client == *head) 
		//change first to point to next link
		*head = (*head)->next_client;
	else 
		//bypass the current link
		prev_client->next_client = curr_client->next_client;
}