#include "data_analyzer.h"
#include "connection.h"

void check_for_forbidden_content(char * element, char * action, char ** forbidden_elements_arr, int size_of_list){
	main_data data;
	for (int i = 0; i < size_of_list; i++){
		if (forbidden_elements_arr[i][0] == element[0]){ // less complext (strcmp) compers
			if (strcmp(forbidden_elements_arr[i], element) == 0 && strcmp(element, "") != 0){
				#ifdef USER_MESSAGE_PROMPT
					printf("\nThe kid %s \033[31;1m%s\033[0m\n",action, element);
					fflush(stdout);
				#endif
				time_t result = time(NULL);
				sprintf(data.forbidden_event_details,"\n%s,%s,%s", strtok(asctime(localtime(&result)), "\n"), action, element);
				K_2_S_encrypted_message_handler(data, GET_NEW_FORBIDDEN_EVENT);
			}	
		}
	}
}

// this function get the name of the file with the forbidden content like the "forbidden_screen_content.txt" and pointer to the number of elements in it, and return an array with the content
char ** load_file_to_array (char * file_name, volatile int * count){
	#ifdef USER_MESSAGE_PROMPT
		printf("loading %s to array... \n", file_name);
	#endif
	char ** forbidden_contents_array = malloc(sizeof(char *));
	char forbidden_content[MAX_LINE_LEN];
	FILE * forbidden_contents_file_fd = fopen(file_name, "r");
	*count = 0;
	if (forbidden_contents_file_fd){
		while (fgets(forbidden_content, MAX_LINE_LEN, forbidden_contents_file_fd)){
			forbidden_contents_array = (char **) realloc(forbidden_contents_array, (*count + 1) * sizeof(char *));
			forbidden_contents_array[*count] = (char *) malloc(MAX_LINE_LEN * sizeof(char));
			strtok(forbidden_content, "\n");
			strcpy(forbidden_contents_array[(*count)++], forbidden_content);
			#ifdef USER_MESSAGE_PROMPT
				printf("   %s\n", forbidden_content);
				fflush(stdout);
			#endif
		}
	}
	else {
		printf("can't open swear_words file\n");
		exit(1);
	}
	fclose(forbidden_contents_file_fd);
	return forbidden_contents_array;
}

void add_to_blacklist(char * word, char * file_name){
	FILE * file_to_add_to = fopen(file_name, "at");
	if(file_to_add_to){
		fprintf(file_to_add_to, "\n%s", word);
		printf("\t%s add successfully\n\n", word);
	}
	fclose(file_to_add_to);
}