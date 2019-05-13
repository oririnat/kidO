#include <stdio.h>
#include <unistd.h>
#include "gui_prompt.h"

#define MAX_DISPLAYS_NUM 32
#define SCREENSHOT_IMAGE_NAME ".temp_screen.jpg"
#define IBM_CLOUD_APIKEY "c1YtuAQcNxjRpV9s1lNXHZw1sBQqtJMynNI2p10HPVnH"

typedef enum {
	CAPTURE_SUCCESSFULLY,
	CAPTURE_FAILED
} SCREENSHOT_STATUS;

extern char ** swear_words_array;
extern volatile int num_of_swear_words;
extern char ** forbidden_screen_contents_array;
extern volatile int num_of_forbidden_screen_contents;

SCREENSHOT_STATUS capture_screenshot();
void screen_classification_and_ocr_analyzer(char * image_path, char * apikey);
void * start_capture_screenshots();