#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include "gui_prompt.h"


CFMachPortRef eventTap;
extern char ** swear_words_array;
extern volatile int num_of_swear_words;

void * start_keystrokes_analyst();
CGEventRef CGEventCallback(CGEventTapProxy, CGEventType, CGEventRef, void*);
static const char * convert_key_code(int);
