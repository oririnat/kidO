#include "start_menu.h"

char kidO_large_logo[] = "\033[0;34m  ,--.    ,--.   ,--. ,-----.\n  |  |,-. `--' ,-|  |'  .-.  '\n  |     / ,--.' .-. ||  | |  | Digital parental control. \n  |  \\  \\ |  |\\ `-' |'  '-'  '\n  `--'`--'`--' `---'  `-----'  Created by Ori Rinat 2019.\033[0m\n\n";

void moving_airplane(){
	int max_spaces = 65;
	char spaces[max_spaces];
	strcpy(spaces, "");
	
	system("clear");
	for (int i = 0; i < max_spaces; i++){
		printf("\033[0;36m%s        _\n%s      -=\\`\\\n%s  |\\ ____\\_\\__\n%s-=\\c`\"\"\"\"\"\"\" \"`)\n%s    `~~~~~/ /~~`\n%s      -==/ /\n%s        '-'\x1B[0m\n\n\n\n\n\n\n\n\n\n\n\n\n",spaces, spaces, spaces, spaces, spaces, spaces, spaces);
		usleep(37000);
		strcat(spaces," ");
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	}
}

void print_large_banner(){
	system("clear");
	system("clear");
	printf("%s",kidO_large_logo);
}

void print_entering_menu(){
	printf("Welcome to kidO, The most advance parental control in the world\n");
	printf("\033[4;37m\033[1m\033[37mOption menu :\033[0m\n\n");	
	printf("[1] \033[96mLog in.\033[0m\n");
	printf("[2] \033[96mRegister.\033[0m\n\n");
}

void print_adding_menu(){
	printf("[1] \033[96mAdd new forbidden word (OCR or Keystrock forbidden word)\033[0m\n");
	printf("[2] \033[96mAdd new forbidden category \033[0m\n\n");
}

void connecting_load(){
	system("clear");
	int trigger = 500;
	int numDots = 4; 
	char prompt[] = "\033[33;1mConnecting\033[0m";
	for (int i = 0; i < 2; ++i) { // Connecting... Connecting... Connecting...
		// Return and clear with spaces, then return and print prompt.
		printf("\r%*s\r%s", (int)(sizeof(prompt) - 1 + numDots), "", prompt);
		fflush(stdout);

		// Print numDots number of dots, one every trigger milliseconds.
		for (int i = 0; i < numDots; i++) {
			usleep(trigger * 1000);
			printf("\033[33;1m.\033[0m");
			fflush(stdout);
		}
	}
	printf("\n");
}