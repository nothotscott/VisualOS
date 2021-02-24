/*
 * File:		sys.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "sys.h"


#include "shell/text.h"
#include "shell/debug.h"

uint64_t (*syshandler_get(uint64_t num))() {
	switch(num) {
		case 1:
			return sys_write;
		case 60:
			return sys_exit;
		default:
			return syshandler_stub;
	}
}

uint64_t sys_write(uint64_t descriptor, char* buffer, size_t size) {
	// TODO file descriptor and all that linuxy crap
	for(size_t i = 0; i < size && buffer[i] != 0; i++){
		text_output_char(buffer[i], TEXT_COLOR_FOREGROUND);
	}
	return 0;
}

uint64_t syshandler_stub(){
	debug_output_warning("sys stub reached\n", false);
	return 0;
}


uint64_t sys_exit(uint64_t error_code) {
	debug_output_info("Exiting userspace\n", true);
	return error_code;
}