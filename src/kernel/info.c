/*
 * File:		info.c
 * Description:	Relay machine information
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include "bootloader.h"
#include "shell/shell.h"
#include "shell/tools.h"
#include "memory/heap.h"
#include "cpu/cpuid.h"


void print_cpu() {
	char* cpu_str = (char*)malloc(64);
	cpuid_get_str(cpu_str, cpu_str + 13);
	print("CPU Vendor: "); print_color(cpu_str, SHELL_COLOR_GREEN); print_newline();
	print("CPU Brand:  "); print_color(cpu_str + 13, SHELL_COLOR_GREEN); print_newline();
	free(cpu_str);
	struct CPUIDFeatures* cpu_features = (struct CPUIDFeatures*)malloc(16);
	print("CPUIDFeatureSet1:   "); print_color("0x", SHELL_COLOR_NUMBER); print_color(string_str_from_ulong((ulong_t)cpu_features->set1 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet2:   "); print_color("0x", SHELL_COLOR_NUMBER); print_color(string_str_from_ulong((ulong_t)cpu_features->set2 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet3:   "); print_color("0x", SHELL_COLOR_NUMBER); print_color(string_str_from_ulong((ulong_t)cpu_features->set3 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet4:   "); print_color("0x", SHELL_COLOR_NUMBER); print_color(string_str_from_ulong((ulong_t)cpu_features->set4 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet5:   "); print_color("0x", SHELL_COLOR_NUMBER); print_color(string_str_from_ulong((ulong_t)cpu_features->set5 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet6:   "); print_color("0x", SHELL_COLOR_NUMBER); print_color(string_str_from_ulong((ulong_t)cpu_features->set6 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSetAMD1: "); print_color("0x", SHELL_COLOR_NUMBER); print_color(string_str_from_ulong((ulong_t)cpu_features->amd1 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSetAMD2: "); print_color("0x", SHELL_COLOR_NUMBER); print_color(string_str_from_ulong((ulong_t)cpu_features->amd2 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();

	print("CPU Supports MMX:    "); print_bool(cpu_features->set1 & CPUID_MMX); print_newline();
	print("CPU Supports SSE:    "); print_bool(cpu_features->set1 & CPUID_SSE); print_newline();
	print("CPU Supports SSE2:   "); print_bool(cpu_features->set1 & CPUID_SSE2); print_newline();
	print("CPU Supports SSE3:   "); print_bool(cpu_features->set1 & CPUID_SSE3); print_newline();
	print("CPU Supports SSE4.1: "); print_bool(cpu_features->set2 & CPUID_SSE4_1); print_newline();
	print("CPU Supports SSE4.2: "); print_bool(cpu_features->set2 & CPUID_SSE4_2); print_newline();
	print("CPU Supports AVX:    "); print_bool(cpu_features->set2 & CPUID_AVX); print_newline();
	print("CPU Supports AVX2:   "); print_bool(cpu_features->set2 & CPUID_AVX2); print_newline();
	print("CPU Supports AVX512: "); print_bool(cpu_features->set3 & CPUID_AVX512_F); print_newline();
	print("CPU Supports 3DNow!: "); print_bool(cpu_features->amd1 & CPUID_3DNOW); print_newline();
	print("CPU Supports Hyper-threading: "); print_bool(cpu_features->set2 & CPUID_HYPERV); print_newline();
	print("CPU Supports Sys-enter/exit:  "); print_bool(cpu_features->set1 & CPUID_SEP); print_newline();
	print("CPU Supports Process context: "); print_bool(cpu_features->set2 & CPUID_PCID); print_newline();
	print("CPU Supports Hardware SHA:    "); print_bool(cpu_features->set3 & CPUID_SHA); print_newline();

	free(cpu_features);
}

void print_info() {
	//print_cpu();
}