#include "entering_to_kidO.h"
#include "connection.h"

int main (){	
	FILE * server_log = fopen("server_log.log", "a");
	if (server_log == NULL)
		printf("Can't opening log file\n");
	
	time_t time_function = time(NULL);
	char * curr_time;
	curr_time = asctime(localtime(&time_function)); // time function
	fprintf(server_log, "kidO server started at : %s", curr_time);
	fflush(server_log);
	
	srand(time(0)); // make the random funtion real random
	
	initialize_connection(); 
	
	int num_of_connected_clients = 0;
	client_ptr curr_client; // poss of link list
	FILE * curr_forbidden_events_fd;
	char name_of_forbidden_events_file[MAX_USER_NAME_LEN + 4];
	encrypted_general_message_protocol message;
	registration_status registering_final_status;
	STATUS loging_in_status;

	
	while(1) {
		time_function = time(NULL);
		curr_time = asctime(localtime(&time_function));	
		strtok(curr_time, "\n");
		//clear the socket set 
		FD_ZERO(&readfds);
		
		//add master socket to set
		FD_SET(master_socket, &readfds); 
		max_socket_descriptor = master_socket;
	
		add_child_sockets_to_set(&clients);
		
		//wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely 
		activity = select( max_socket_descriptor + 1 , &readfds , NULL , NULL , NULL); 
		if ((activity < 0) && (errno != EINTR)) {
			fprintf(server_log, "%s : select error", curr_time);
			fflush(server_log);
		}
				
		//If something happened on the master socket, then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds)) { 
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) { 
				fprintf(server_log, "%s : accept error", curr_time);
				fflush(server_log);
				exit(EXIT_FAILURE);
				
			}
			fprintf(server_log,"%s : New connection , socket fd is %d , ip is : %s , port : %d\n",curr_time, new_socket, inet_ntoa(address.sin_addr) , ntohs (address.sin_port));
			fflush(server_log);
			
			//add new client to client link list
			insert_client(new_socket, "", &clients);
			num_of_connected_clients++;
			print_clients_list(clients);
			
			fprintf(server_log, "%s : Adding to list of sockets as %d\n", curr_time, new_socket);
			fflush(server_log);
		}
		
		//else its some IO operation on some other socket 
		for (int i = 0; i < num_of_connected_clients; i++){
			if(curr_client == NULL)
				curr_client = clients; // if way get to the end of the clients link list -> go to the head of the link list
			
			socket_descriptor = curr_client->socket_fd;
			
			if (FD_ISSET( socket_descriptor , &readfds)) {
				//Check if it was for closing , and also read the incoming message
				if (read(socket_descriptor, &message, sizeof(encrypted_general_message_protocol)) == 0){ 
					//Somebody disconnected , get his details and print
					getpeername(socket_descriptor , (struct sockaddr*)&address ,(socklen_t*)&addrlen); 
					fprintf(server_log,"%s : Host disconnected, ip %s , port %d \n",curr_time, inet_ntoa(address.sin_addr), ntohs(address.sin_port)); 
					fflush(server_log);
					
					//Close the socket and mark as 0 in list for urses 
					close(socket_descriptor);
					remove_client(socket_descriptor, &clients); // removeing the disconnected client by his socket_descriptor
					num_of_connected_clients --;
					print_clients_list(clients);
				}
				else {
					switch (message.action) {
						case LOG_IN :
							loging_in_status = 
							log_in_parent(decrypt_text(message.data.encrypted_login.username, curr_client->encryption_key), decrypt_text(message.data.encrypted_login.password, curr_client->encryption_key));
							send(socket_descriptor, &loging_in_status, sizeof(registration_status), 0);
							if (loging_in_status == SUCCESS){// if attacker loged in -connect he's id the hes socket descriptor
								print_clients_list(clients);
								strcpy(curr_client->name, decrypt_text(message.data.encrypted_login.username, curr_client->encryption_key));
							}
							
							break;
							
						case REGISTER :
							registering_final_status = register_new_parent(decrypt_text(message.data.encrypted_register_parent.username, curr_client->encryption_key), decrypt_text(message.data.encrypted_register_parent.password, curr_client->encryption_key));
							
							send(socket_descriptor, &registering_final_status, sizeof(registration_status),0);
							
							break;

						case KEY_EXCHANGE :
							recv_parent_aes_key(&curr_client, message.data.key_exchange_buffer);
							
							break ;
						
						case GET_NEW_FORBIDDEN_EVENT : 
							sprintf(name_of_forbidden_events_file, "../web/files/%s.csv", curr_client->name);
							curr_forbidden_events_fd = fopen(name_of_forbidden_events_file, "r");
							if (curr_forbidden_events_fd == NULL ){
								fclose(curr_forbidden_events_fd);
								curr_forbidden_events_fd = fopen(name_of_forbidden_events_file, "a");
								fprintf(curr_forbidden_events_fd, "time,event,details");
								fprintf(curr_forbidden_events_fd, "%s", decrypt_text(message.data.encrypted_forbidden_event_details, curr_client->encryption_key));
							}
							else{
								curr_forbidden_events_fd = fopen(name_of_forbidden_events_file, "a");
								fprintf(curr_forbidden_events_fd, "%s", decrypt_text(message.data.encrypted_forbidden_event_details, curr_client->encryption_key));

							}
							fflush(curr_forbidden_events_fd);
							fclose(curr_forbidden_events_fd);
							break;
							
						default :
							
							break;	
						
					}
				}
				curr_client = curr_client->next_client;
			}
		} 
	}
	fclose(server_log);
	return 0; 
}