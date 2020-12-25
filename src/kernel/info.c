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
	print("CPU Vendor: ", SHELL_COLOR_FOREGROUND); print(cpu_str, SHELL_COLOR_GREEN); print_newline();
	print("CPU Brand:  ", SHELL_COLOR_FOREGROUND); print(cpu_str + 13, SHELL_COLOR_GREEN); print_newline();
	free(cpu_str);
	struct CPUIDFeatures* cpu_features = (struct CPUIDFeatures*)malloc(16);
	print("CPUIDFeatureSet1:   ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_NUMBER); print(string_str_from_ulong((ulong)cpu_features->set1 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet2:   ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_NUMBER); print(string_str_from_ulong((ulong)cpu_features->set2 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet3:   ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_NUMBER); print(string_str_from_ulong((ulong)cpu_features->set3 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet4:   ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_NUMBER); print(string_str_from_ulong((ulong)cpu_features->set4 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet5:   ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_NUMBER); print(string_str_from_ulong((ulong)cpu_features->set5 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSet6:   ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_NUMBER); print(string_str_from_ulong((ulong)cpu_features->set6 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSetAMD1: ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_NUMBER); print(string_str_from_ulong((ulong)cpu_features->amd1 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();
	print("CPUIDFeatureSetAMD2: ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_NUMBER); print(string_str_from_ulong((ulong)cpu_features->amd2 & 0xffffffff), SHELL_COLOR_NUMBER); print_newline();

	print("CPU Supports MMX:    ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set1 & CPUID_MMX); print_newline();
	print("CPU Supports SSE:    ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set1 & CPUID_SSE); print_newline();
	print("CPU Supports SSE2:   ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set1 & CPUID_SSE2); print_newline();
	print("CPU Supports SSE3:   ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set1 & CPUID_SSE3); print_newline();
	print("CPU Supports SSE4.1: ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set2 & CPUID_SSE4_1); print_newline();
	print("CPU Supports SSE4.2: ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set2 & CPUID_SSE4_2); print_newline();
	print("CPU Supports AVX:    ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set2 & CPUID_AVX); print_newline();
	print("CPU Supports AVX2:   ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set2 & CPUID_AVX2); print_newline();
	print("CPU Supports AVX512: ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set3 & CPUID_AVX512_F); print_newline();
	print("CPU Supports 3DNow!: ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->amd1 & CPUID_3DNOW); print_newline();
	print("CPU Supports Hyper-threading: ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set2 & CPUID_HYPERV); print_newline();
	print("CPU Supports Sys-enter/exit:  ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set1 & CPUID_SEP); print_newline();
	print("CPU Supports Process context: ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set2 & CPUID_PCID); print_newline();
	print("CPU Supports Hardware SHA:    ", SHELL_COLOR_FOREGROUND); print_bool(cpu_features->set3 & CPUID_SHA); print_newline();

	free(cpu_features);
}

void print_info() {
	print_cpu();
}