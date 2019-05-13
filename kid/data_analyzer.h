#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "gui_prompt.h"

#define MAX_LINE_LEN 40
#define SWEAR_WORDS_FILE_NAME ".swear_words.txt"
#define FORBIDDEN_SCREEN_CONTENTS_FILE_NAME ".forbidden_screen_content.txt" 

void check_for_forbidden_content(char * element, char * action, char ** forbidden_elements_arr, int size_of_list);
char ** load_file_to_array(char * file_name, volatile int * count);
void add_to_blacklist(char * word, char * file_name);