/*
 * File:		symbol.h
 * Description:	Gives the ability for the kernel to introspectively look at its
 * 				own symbols
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define SYMBOL_FIND_FAIL	-1

struct SymbolMap {
	uint64_t	address;
	size_t		symbol_index;
} __attribute__((packed));

struct SymbolTableInformation {
	struct ELF64SectionHeader*	section_headers;
	size_t						section_headers_num;
	struct ELF64Symbol*			symbol_entries;
	size_t						symbol_entries_num;
	char*						symbol_strings;
	size_t						symbol_strings_size;
	char*						section_strings;
	size_t						section_strings_size;
	struct SymbolMap*			symbol_map;
	size_t						symbol_map_num;
};

// Initializes locating the symbol table from the kernel header the passed by the bootloader
void symbol_init();

// Looks up the symbol for [address] and write the pointers to [symbol_str] and optionally [section_str]. Returns true on success
bool symbol_lookup(void* address, char** symbol_str, char** section_str);