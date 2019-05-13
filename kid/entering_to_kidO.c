#include "entering_to_kidO.h"
#include "connection.h"
#include "utilities.h"
#include "md5.h"

char parent_username[MAX_USER_NAME_LEN];
ENTERING_STATUS log_in(){
	ENTERING_STATUS received_status;
	main_data data;
	printf("\033[4;37m\033[1m\033[37mLog in -\033[0m\n");
	
	printf("Enter your user name : ");
	safe_scan(&data.login.username, MAX_USER_NAME_LEN);
	strcpy(parent_username, data.login.username);

	printf("Enter you password : ");
	safe_scan(&data.login.password, MAX_PASSWORD_LEN);
	
	K_2_S_encrypted_message_handler(data, LOG_IN);
	
	recv(K_S_socket, &received_status, sizeof(registration_status), 0);
	if(received_status == ENTERING_ACTION_SUCCESS){
//		print_the_Hitchiker_image();
		printf("\n[\033[32;1m+\033[0m] \033[32;1mLogged in successfully !\033[0m\n\n");
		#ifdef USER_MESSAGE_PROMPT
		sleep(4);
		#endif
		return ENTERING_ACTION_SUCCESS;
	}
	printf("\n[\033[31;1m-\033[0m] \033[31;1mUnable to log in, please try again ...\033[0m\n");
	sleep(2);
	
	return ENTERING_ACTION_FAILURE;
}

ENTERING_STATUS register_parent(){
	registration_status received_status;
	main_data data;
	char password_verification_input[MAX_PASSWORD_LEN];
	
	printf("\033[4;37m\033[1m\033[37mRegister -\033[0m\n");
		
	printf("Enter your user name (no spaces) : ");
	safe_scan(&data.register_parent.username, MAX_USER_NAME_LEN);
		
	printf("Enter your password : ");
	safe_scan(&data.register_parent.password, MAX_PASSWORD_LEN);
		
	printf("verify your password : ");
	safe_scan(&password_verification_input, MAX_PASSWORD_LEN);
	
	if(strcmp(data.register_parent.password, password_verification_input) != 0){
		printf("\n[\033[31;1m-\033[0m] \033[31;1mPassword verification invalid, try again...\033[0m\n");
		sleep(2);
			
		return ENTERING_ACTION_FAILURE;
	}
	K_2_S_encrypted_message_handler(data, REGISTER);
	recv(K_S_socket, &received_status, sizeof(registration_status),0);
	
	switch (received_status) {
		case REGISTERED_SUCCESSFULLY :
			
			printf("\n[\033[32;1m+\033[0m] \033[32;1mYou registered successfully !\033[0m\n");
			press_enter_to_continue();
			
			return ENTERING_ACTION_SUCCESS;
			
			break;
			
		case USERNAME_TAKEN :
			printf("\n[\033[31;1m-\033[0m] \033[31;1mUser name taken, please try again...\033[0m\n");
			sleep(3);
			
			return ENTERING_ACTION_FAILURE;
			
			break;
		
		case INVALID_USERNAME :
			
			printf("\n[\033[31;1m-\033[0m] \033[31;1mUser name is invalid , please try again...\033[0m\n");
			sleep(3);
			
			return ENTERING_ACTION_FAILURE;
			
			break;	
			
		default:
			printf("\n[\033[31;1m-\033[0m] \033[31;1mRegistered error...\033[0m\n");
			sleep(3);
			
			return ENTERING_ACTION_FAILURE;
			
			break;
	}
	return ENTERING_ACTION_FAILURE;
}