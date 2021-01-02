/*
 * File:		kernel.h
 * Description:	Kernel-specific and auto-included definitions
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define NULL	0


struct InterruptStack
{
	ulong_t	rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
	ulong_t error_code, rip, cs, rflags, rsp, ss;
};

// From kernel.ld linker script
extern void** _kernel_start;
extern void** _kernel_end;


// Porta-debug
/*
#include <string.h>
#include "shell/shell.h"
#include "shell/tools.h"
*/