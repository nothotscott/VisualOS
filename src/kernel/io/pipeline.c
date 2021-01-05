/*
 * File:		pipeline.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include <queue.h>
#include "shell/shell.h"
#include "shell/tools.h"
#include "pipeline.h"

struct Queue* g_pipeline;


void double_fault_handler(struct InterruptStack* stack, size_t num) {
	print_newline();
	print("DOUBLE FAULT DETECTED", SHELL_COLOR_RED); print_newline();
	print("  ERROR CODE: 0x", SHELL_COLOR_RED); print(string_str_from_ulong(stack->error_code), SHELL_COLOR_RED); print_newline();
	while(true);
}

void general_protection_fault_handler(struct InterruptStack* stack, size_t num) {
	print_newline();
	print("GENERAL PROTECTION FAULT DETECTED", SHELL_COLOR_RED); print_newline();
	print("  ERROR CODE: 0x", SHELL_COLOR_RED); print(string_str_from_ulong(stack->error_code), SHELL_COLOR_RED); print_newline();
	while(true);
}

void paging_fault_handler(struct InterruptStack* stack, size_t num) {
	print_newline();
	print("PAGE FAULT DETECTED", SHELL_COLOR_RED); print_newline();
	print("  INTERRUPT NUM: ", SHELL_COLOR_RED); print(string_str_from_int(num), SHELL_COLOR_RED); print_newline();
	print("  ERROR CODE:    0x", SHELL_COLOR_RED); print(string_str_from_ulong(stack->error_code), SHELL_COLOR_RED); print_newline();
	print("  REGISTER RIP:  0x", SHELL_COLOR_RED); print(string_str_from_ulong(stack->rip), SHELL_COLOR_RED); print_newline();
	while(true);
}