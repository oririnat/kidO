#include "start_menu.h"
#include <string.h>
#include <stdbool.h>

typedef enum {
	REGISTERED_SUCCESSFULLY,
	USERNAME_TAKEN,
	INVALID_USERNAME,
	PASSWORD_VERIFICTION_INVALID
} registration_status;

typedef enum {
	ENTERING_ACTION_SUCCESS,
	ENTERING_ACTION_FAILURE
} ENTERING_STATUS;

ENTERING_STATUS log_in();
ENTERING_STATUS register_parent();
void buy_license_key();
void copytoclipboard(const char * str);
bool all_dependent_program_is_installed();