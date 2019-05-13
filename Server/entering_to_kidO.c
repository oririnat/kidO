#include "entering_to_kidO.h"
#include "md5.h"

//* registration handling  *//
bool valid_parent_name (char * victim_name);

registration_status register_new_parent(char username_input[], char hashed_password_input[]) {
	if (valid_parent_name(username_input)){
		if (add_parent_to_DB(username_input, hashed_password_input) == FAILUR)
			return USERNAME_TAKEN;
	}
	else
		return INVALID_USERNAME;

	return REGISTERED_SUCCESSFULLY;
}

bool valid_parent_name (char * victim_name){
	for (int i = 0; i < strlen(victim_name); i++){
		if (!((victim_name[i] >= 'a' && victim_name[i] <= 'z') || (victim_name[i] >= 'A' && victim_name[i] <= 'Z') || victim_name[i] == '_' || (victim_name[i] >= '0' && victim_name[i] <= '9')))
			return false;
	}
	return true;
}

STATUS add_parent_to_DB(char username_input[], char hashed_password_input[]){	
	FILE * parents_DB_fd;
	
	if(parent_exsist_in_DB(username_input))
		return FAILUR;
	parents_DB_fd = fopen(PARENTS_DB_NAME, "at");
	if(parents_DB_fd == NULL)
		return FAILUR;
	
	fprintf(parents_DB_fd, "\n%s,%s",username_input, hashed_password_input);
	fclose(parents_DB_fd);
	return SUCCESS;
}

int parent_exsist_in_DB(char username_input[]){
	FILE * parents_DB_fd;
	char curr_parent_line[HASH_LEN + MAX_USER_NAME_LEN + 2];
	parents_DB_fd = fopen(PARENTS_DB_NAME, "rt");
	if(parents_DB_fd ==  NULL)
		return 0;
	while(fgets(curr_parent_line, sizeof(curr_parent_line), parents_DB_fd))
		if(strcmp(get_csv_field(curr_parent_line, USER_NAME_FIELD), username_input) == 0)
			return 1;
	
	fclose(parents_DB_fd);
	return 0;
}
//* end registration handling  *//


//* log in handling  *//
STATUS log_in_parent(char username_input[], char hashed_password_input[]){
	FILE * parents_DB_fd;
	char curr_parent_line[HASH_LEN + MAX_USER_NAME_LEN + 2];
	parents_DB_fd = fopen(PARENTS_DB_NAME,"rt");
	while(fgets(curr_parent_line, sizeof(curr_parent_line), parents_DB_fd))
		if((strcmp(get_csv_field(curr_parent_line, USER_NAME_FIELD), username_input) == 0) && (strcmp(get_csv_field(curr_parent_line, PASSWORD_FIELD), hashed_password_input) == 0)){
			return SUCCESS;
		}	
	
	return FAILUR;
	fclose(parents_DB_fd);
}
//* end log in handling  *//

//* utilities *//

char * get_csv_field(char * line, int num){
	char * tok;
	char * temp_line = strdup(line);
	for (tok = strtok(temp_line, ",");
			tok && *tok;
			tok = strtok(NULL, ",\n"))
	{
		if (!--num)
			return tok;
	}
	free(temp_line);
	return NULL;
}
//* end utilities *//