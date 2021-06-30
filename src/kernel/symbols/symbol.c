/*
 * File:		symbol.c
 * TODO:		move this into userspace
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "log.h"
#include "bootloader.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/pageframe.h"
#include "elf.h"
#include "symbol.h"


// Resolves a symbol by finding the closest symbol in the symbol map for [address] and returning the index.
static size_t symbol_resolve(uint64_t address);

static struct SymbolTableInformation s_symbol_table;

void symbol_init() {
	struct ELF64Header* header = bootloader_get_info()->kernel_header;
	pageframe_reserve_size(header, header->e_ehsize);
	struct ELF64SectionHeader* section_headers = (struct ELF64SectionHeader*)((void*)header + header->e_shoff);
	size_t section_headers_num = header->e_shnum;
	size_t section_string_index = header->e_shstrndx;
	pageframe_reserve_size(section_headers, section_headers_num * header->e_shentsize);
	s_symbol_table.section_headers = section_headers;
	s_symbol_table.section_headers_num = section_headers_num;
	// Get the symbol table info
	for(size_t i = 0; i < section_headers_num; i++) {
		struct ELF64SectionHeader* section_header = section_headers + i;
		if(section_header->sh_type == ELF_SECTION_HEADER_TYPE_SYMTAB) {
			void* symbol_table = ((void*)header + section_header->sh_offset);
			size_t symbol_table_entries_num = section_header->sh_size / section_header->sh_entsize;
			pageframe_reserve_size(symbol_table, section_header->sh_size);
			s_symbol_table.symbol_entries = symbol_table;
			s_symbol_table.symbol_entries_num = symbol_table_entries_num;
		} else if(section_header->sh_type == ELF_SECTION_HEADER_TYPE_STRTAB) {
			void* string_table = ((void*)header + section_header->sh_offset);
			size_t string_table_size = section_header->sh_size;
			pageframe_reserve_size(string_table, string_table_size);
			if(section_string_index == i) {
				//log_default("Section string table located, offset: 0x%x\n", section_header->sh_offset);
				s_symbol_table.section_strings = string_table;
				s_symbol_table.section_strings_size = string_table_size;
			} else {
				//log_default("Symbol string table located, offset: 0x%x\n", section_header->sh_offset);
				s_symbol_table.symbol_strings = string_table;
				s_symbol_table.symbol_strings_size = string_table_size;
			}
		}
	}
	// Get the size of symbol map and allocate
	size_t symbol_map_num = 0;
	for(size_t i = 0; i < s_symbol_table.symbol_entries_num; i++) {
		struct ELF64Symbol* symbol = s_symbol_table.symbol_entries + i;
		if(symbol->st_value == 0) {
			continue;
		}
		symbol_map_num++;
	}
	size_t symbol_map_size = symbol_map_num * sizeof(struct SymbolMap);
	struct SymbolMap* symbol_map = pageframe_request_pages(NEAREST_PAGE(symbol_map_size));
	// Filter and order symbol map (insertion sort)
	size_t ai = 0;	// adjusted i
	for(size_t i = 0; i < s_symbol_table.symbol_entries_num; i++) {
		// real hardware crashes here?
		struct ELF64Symbol* symbol = s_symbol_table.symbol_entries + i;
		uint64_t value = symbol->st_value;
		if(value == 0) {
			continue;
		}
		size_t j = ai - 1;
		if(ai >= 1 ){
			while(j >= 0 && symbol_map[j].address > value) {
				symbol_map[j + 1] = symbol_map[j];
				j--;
			}
		}
		symbol_map[j + 1] = (struct SymbolMap){
			.address = value,
			.symbol_index = i
		};
		ai++;
	}
	s_symbol_table.symbol_map = symbol_map;
	s_symbol_table.symbol_map_num = symbol_map_num;
}

// TODO support virtual addresses. Reverse paging?

static size_t symbol_resolve(uint64_t address) {
	size_t max = s_symbol_table.symbol_map_num - 1;
	size_t low = 0;
	size_t high = max;
	while(low <= high && low <= max - 1) {
		size_t mid = (low + high) / 2;
		uint64_t value = s_symbol_table.symbol_map[mid].address;
		uint64_t value_next = s_symbol_table.symbol_map[mid + 1].address;
		if(value <= address && value_next > address) {
			return mid;
		}
		if(value > address) {	// value too big, must be in left subarray
			high = mid - 1;
		} else {				// value too small, must be in right subarray
			low = mid + 1;
		}
	}
	if (low > max - 1) {
		return max;
	}
	return SYMBOL_FIND_FAIL;
}

bool symbol_lookup(void* address, char** symbol_str, char** section_str) {
	long kernel_offset = bootloader_get_info()->kernel_offset;
	address -= kernel_offset;
	size_t map_index = symbol_resolve((uint64_t)address);
	if(map_index == SYMBOL_FIND_FAIL) {
		return false;
	}
	size_t symbol_index = s_symbol_table.symbol_map[map_index].symbol_index;
	struct ELF64Symbol* symbol = s_symbol_table.symbol_entries + symbol_index;
	size_t string_index = symbol->st_name;
	if(string_index == 0) {
		return false;
	}
	*symbol_str = s_symbol_table.symbol_strings + string_index;
	if(section_str != NULL) {
		struct ELF64SectionHeader* section_header = s_symbol_table.section_headers + symbol->st_shndx;
		*section_str = s_symbol_table.section_strings + section_header->sh_name;
	}
	return true;
}