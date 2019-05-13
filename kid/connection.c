#include "connection.h"
#include <time.h>

STATUS create_connection(){
	struct sockaddr_in serverAddr;
	K_S_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(K_S_socket < 0)
		return FAILUR;
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	if(connect(K_S_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		return FAILUR;
	return SUCCESS;
}

void secure_key_exchange(){
	encrypted_general_message_protocol encrypted_message;
	encrypted_message.action = KEY_EXCHANGE;
	
	FILE * rsa_result_fd;
	char sub_buffer[65];
	char encrypt_commend[AES_KEY_LEN + 90];
	memset(encrypted_message.data.key_exchange_buffer, '\0', RSA_BASE64_AES_KEY_SIZE); // this is a aes key then encypted in rsa
	memset(sub_buffer, '\0', sizeof(sub_buffer));
	
	strcpy(AES_KEY, generate_key("XXXXXXXXXXXXXXXX")); // aes key len is 16 -> X 16 times
	sprintf(encrypt_commend, "echo %s | openssl rsautl -encrypt -inkey public_key.pem -pubin | openssl base64", AES_KEY);
	#ifdef USER_MESSAGE_PROMPT
		printf("curr AES key : %s\n", AES_KEY);
	#endif
	rsa_result_fd = popen(encrypt_commend, "r");
	while (fgets(sub_buffer, sizeof(sub_buffer), rsa_result_fd) != NULL)
		strcat(encrypted_message.data.key_exchange_buffer, sub_buffer);
	send(K_S_socket, &encrypted_message, sizeof(encrypted_general_message_protocol), 0);	
	pclose(rsa_result_fd);
}
//
//void secure_key_exchange(){ // send kid's public RSA key to the server, then the server can transfer to this victim his attacker aes key
//	encrypted_general_message_protocol encrypted_message;
//	encrypted_message.i_am = VICTIM;
//	encrypted_message.action = KEY_EXCHANGE;
//	FILE * public_key_file = fopen("public_key.pem", "r");
//	FILE * encrepted_aes_key_fd;
//	FILE * rsa_decryption_fd;
//	char sub_buffer[65];
//	char encrypted_aes_key_result[RSA_BASE64_AES_KEY_SIZE];
//	char decrypt_rsa_cmd[90];
//	char delete_temp_aes_key_cmd[5 + sizeof(TEMP_RCVE_AES_KEY_FILE_NAME)];
//	
//	memset(encrypted_message.data.key_exchange_buffer, '\0', sizeof(encrypted_message.data.key_exchange_buffer));
//	memset(sub_buffer, '\0', sizeof(sub_buffer));
//	
//	while (fgets(sub_buffer, sizeof(sub_buffer), public_key_file) != NULL) // load the public_key.pem file to the buffer
//		strcat(encrypted_message.data.key_exchange_buffer, sub_buffer);	
//	send(K_S_socket, &encrypted_message, sizeof(encrypted_general_message_protocol), 0);
//	fclose(public_key_file);
//	
//	recv(K_S_socket, &encrypted_aes_key_result, sizeof(encrypted_aes_key_result), 0);
//	encrepted_aes_key_fd = fopen(TEMP_RCVE_AES_KEY_FILE_NAME, "w");
//	fprintf(encrepted_aes_key_fd, "%s", encrypted_aes_key_result);
//	fclose(encrepted_aes_key_fd);
//	
//	sprintf(decrypt_rsa_cmd, "openssl base64 -d -in %s | openssl rsautl -decrypt -inkey private_key.pem", TEMP_RCVE_AES_KEY_FILE_NAME);
//	rsa_decryption_fd = popen(decrypt_rsa_cmd, "r");
//	fgets(AES_KEY, sizeof(AES_KEY), rsa_decryption_fd);	
//	pclose(rsa_decryption_fd);
//	
//	// delete "TEMP_RCVE_AES_KEY_FILE_NAME" file
//	sprintf(delete_temp_aes_key_cmd, "rm %s", TEMP_RCVE_AES_KEY_FILE_NAME);
//	system(delete_temp_aes_key_cmd);
//}

// kid coputer to server encrypted message handler, get main_data struct and encrypt it in aes 256 encryption
void K_2_S_encrypted_message_handler(main_data data, action_type action){ // kid - server 
	encrypted_general_message_protocol encrypted_message;
	encrypted_message.action = action;
	
	switch (action) {
		
		case LOG_IN :
			strcpy(encrypted_message.data.encrypted_login.username, encrypt_text(data.login.username, AES_KEY));
			strcpy(encrypted_message.data.encrypted_login.password, encrypt_text(md5(data.login.password), AES_KEY));
			
			break;
			
		case REGISTER :
			strcpy(encrypted_message.data.encrypted_register_parent.username, encrypt_text(data.register_parent.username, AES_KEY));
			strcpy(encrypted_message.data.encrypted_register_parent.password, encrypt_text(md5(data.register_parent.password), AES_KEY));
			
			break;
			
		case GET_NEW_FORBIDDEN_EVENT :
			strcpy(encrypted_message.data.encrypted_forbidden_event_details, encrypt_text(data.forbidden_event_details ,AES_KEY));
		break;
		
		default :
			break;
	}
	send(K_S_socket, &encrypted_message, sizeof(encrypted_message), 0);
}

//// send a file in plain text or in AES-256-CBC encrypted way
//void send_file (char * file_name, action_type action, bool convert_to_base64_and_encrypt) {
//	main_data data;
//	char sub_buffer[MTU];
//	FILE * encrypted_file_fd;
//	char convert_file_to_buffer_commend[MAX_FILE_TO_BUFFER_COMMEND_LEN];
//	
//	char temp_checksum_full_buffer[MTU + HASH_LEN];
//	char temp_checksum[HASH_LEN];
//	strcpy(temp_checksum, "\0");
//	memset(temp_checksum_full_buffer, '0', strlen(temp_checksum_full_buffer));
//	data.file_data.end_of_file = 0;
//	
//	if (convert_to_base64_and_encrypt)
//		sprintf(convert_file_to_buffer_commend, "openssl aes-256-cbc -base64 -in %s -k %s", file_name, AES_KEY); // there is injection variability, to be fix
//	else 
//		sprintf(convert_file_to_buffer_commend, "cat %s", file_name);
//	
//	encrypted_file_fd = popen(convert_file_to_buffer_commend, "r");
//	
//	while (fgets(sub_buffer, sizeof(sub_buffer), encrypted_file_fd) != NULL) {
//		strcpy(temp_checksum_full_buffer, temp_checksum);
//		strcat(temp_checksum_full_buffer, sub_buffer);
//		strcpy(temp_checksum, md5(temp_checksum_full_buffer));
//		strcpy(data.file_data.file_sub_buffer, sub_buffer);
//		V_2_S_encrypted_message_handler(data, GET_SYSTEM_PROFILER);
//	}
//	data.file_data.end_of_file = 1;
//	strcpy(data.file_data.checksum, temp_checksum);
//	V_2_S_encrypted_message_handler(data, GET_SYSTEM_PROFILER);
//	pclose(encrypted_file_fd);
//}


// AES 256 cbc encryption
char * encrypt_text (char * text_to_encrypt, char * encrypt_key){
	int max_len = (strlen(text_to_encrypt) * 1.36) + 100;
	char * encrypted_text = (char *) malloc(max_len * sizeof(char));
	char sub_buffer[65];
	char encrypt_commend[40 + strlen(text_to_encrypt) + strlen(encrypt_key)];
	FILE * temp_aes_file = fopen(ENCRYPTED_RECEIVED_DATA_NAME, "w");
	fprintf(temp_aes_file, "%s", text_to_encrypt);
	fclose(temp_aes_file);
	sprintf(encrypt_commend, "openssl aes-256-cbc -base64 -in %s -k %s", ENCRYPTED_RECEIVED_DATA_NAME, encrypt_key); 
	
	FILE * aes_encryption_fd = popen(encrypt_commend, "r");
	while (fgets(sub_buffer, sizeof(sub_buffer), aes_encryption_fd) != NULL) 
		strcat(encrypted_text, sub_buffer);
	
	pclose(aes_encryption_fd);
	return encrypted_text;
}
//AES 256 cbc decryption
char * decrypt_text (char * text_to_decrypt, char * decryption_key){
	char * decrypted_text = (char *) malloc(strlen(text_to_decrypt) * sizeof(char));
	char sub_buffer[65];
	char decrypt_commend[40 + strlen(text_to_decrypt) + strlen(decryption_key)];
	
	FILE * temp_aes_file = fopen(ENCRYPTED_RECEIVED_DATA_NAME, "w");
	fprintf(temp_aes_file, "%s", text_to_decrypt);
	fclose(temp_aes_file);
	sprintf(decrypt_commend, "openssl aes-256-cbc -d -in %s -base64 -k %s", ENCRYPTED_RECEIVED_DATA_NAME, decryption_key);
	
	FILE * aes_decryption_fd = popen(decrypt_commend, "r");
	while (fgets(sub_buffer, sizeof(sub_buffer), aes_decryption_fd) != NULL) {
//		strtok(sub_buffer, "\n");
		strcat(decrypted_text, sub_buffer);
	}	
	pclose(aes_decryption_fd);
	return decrypted_text;
}

char * generate_key(char * key_template){ // exemple :  key_template = XXXXXXXXXXXX OR XXXX-XXXX-XXXX-XXXX
	char key_dictionary[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
	char * key = (char *) malloc(strlen(key_template) * sizeof(char));
	srand(time(0)); // make the random funtion real random
	for (int i = 0; i < strlen(key_template); i++) 
		key[i] = key_template[i] == 'X' ? key_dictionary[(rand() % strlen(key_dictionary))] : key_template[i];
		
	key[strlen(key_template)] = '\0';
	return key;
}