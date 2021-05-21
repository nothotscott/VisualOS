/*
 * File:		elf.h
 * Description:	Used for parsing ELF files
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

enum ELFSectionHeaderType {
	ELF_SECTION_HEADER_TYPE_NULL			= 0x00,
	ELF_SECTION_HEADER_TYPE_PROGBITS		= 0x01,
	ELF_SECTION_HEADER_TYPE_SYMTAB			= 0x02,
	ELF_SECTION_HEADER_TYPE_STRTAB			= 0x03,
	ELF_SECTION_HEADER_TYPE_RELA			= 0x04,
	ELF_SECTION_HEADER_TYPE_HASH			= 0x05,
	ELF_SECTION_HEADER_TYPE_DYNAMIC			= 0x06,
	ELF_SECTION_HEADER_TYPE_NOTE			= 0x07,
	ELF_SECTION_HEADER_TYPE_NOBITS			= 0x09,
	ELF_SECTION_HEADER_TYPE_REL				= 0x0a,
	ELF_SECTION_HEADER_TYPE_SHLIB			= 0x0b,
	ELF_SECTION_HEADER_TYPE_DYNSYM			= 0x0c,
	ELF_SECTION_HEADER_TYPE_INIT_ARRAY		= 0x0e,
	ELF_SECTION_HEADER_TYPE_FINI_ARRAY		= 0x0f,
	ELF_SECTION_HEADER_TYPE_PREINIT_ARRAY	= 0x10,
	ELF_SECTION_HEADER_TYPE_GROUP			= 0x11,
	ELF_SECTION_HEADER_TYPE_SYMTAB_SHNDX	= 0x12,
	ELF_SECTION_HEADER_TYPE_NUM				= 0x13,
	ELF_SECTION_HEADER_TYPE_LOOS			= 0x60000000
};

enum ELFSymbolInfoType {
	ELF_SYMBOL_INFO_TYPE_NOTYPE			= 0x00,
	ELF_SYMBOL_INFO_TYPE_OBJECT			= 0x01,
	ELF_SYMBOL_INFO_TYPE_FUNC			= 0x02,
	ELF_SYMBOL_INFO_TYPE_SECTION		= 0x03,
	ELF_SYMBOL_INFO_TYPE_FILE			= 0x04,
	ELF_SYMBOL_INFO_TYPE_COMMON			= 0x05,
	ELF_SYMBOL_INFO_TYPE_TLS			= 0x06,
	ELF_SYMBOL_INFO_TYPE_LLOS			= 0x0a,
	ELF_SYMBOL_INFO_TYPE_HIOS			= 0x0c,
	ELF_SYMBOL_INFO_TYPE_LOPROC			= 0x0d,
	ELF_SYMBOL_INFO_TYPE_SPARC_REGISTER	= 0x0e,
	ELF_SYMBOL_INFO_TYPE_HIPROC			= 0x0f
};
enum ELFSymbolInfoBind {
	ELF_SYMBOL_INFO_BIND_LOCAL			= 0x00,
	ELF_SYMBOL_INFO_BIND_GLOAL			= 0x10,
	ELF_SYMBOL_INFO_BIND_WEAK			= 0xa0,
	ELF_SYMBOL_INFO_BIND_LOOS			= 0xc0,
	ELF_SYMBOL_INFO_BIND_HIOS			= 0xd0,
	ELF_SYMBOL_INFO_BIND_LOPROC			= 0xe0,
	ELF_SYMBOL_INFO_BIND_HIPROC			= 0xf0
};

enum ELFSymbolVisibility {	// in the st_other
	ELF_SYMBOL_VISIBILITY_DEFAULT	= 0x00,
	ELF_SYMBOL_VISIBILITY_INTERNAL	= 0x01,
	ELF_SYMBOL_VISIBILITY_HIDDEN	= 0x02,
	ELF_SYMBOL_VISIBILITY_PROTECTED	= 0x03,
	ELF_SYMBOL_VISIBILITY_EXPORTED	= 0x04,
	ELF_SYMBOL_VISIBILITY_SINGLETON	= 0x05,
	ELF_SYMBOL_VISIBILITY_ELIMINATE	= 0x06
};

struct ELF64Header {
	uint8_t		e_ident[16];	// magic
	uint16_t	e_type;			// object type
	uint16_t	e_machine;		// machine ISA
	uint32_t	e_version;		// version
	uint64_t	e_entry;		// memory address of the entry point from where the process starts executing
	uint64_t	e_phoff;		// points to the start of the program header table
	uint64_t	e_shoff;		// points to the start of the section header table
	uint32_t	e_flags;		// interpretation of this field depends on the target architecture
	uint16_t	e_ehsize;		// size of this header
	uint16_t	e_phentsize;	// size of program header table entry
	uint16_t	e_phnum;		// number of entries in the program header table
	uint16_t	e_shentsize;	// size of section header table entry
	uint16_t	e_shnum;		// number of entries in the section header table
	uint16_t	e_shstrndx;		// Index of the section header table entry that contains the section names
} __attribute__((packed));

struct ELF64ProgramHeader {
	uint32_t	p_type;		// type of the segment
	uint32_t	p_flags;	// segment-dependent flags (position)
	uint64_t	p_offset;	// offset of the segment in the file image
	uint64_t	p_vaddr;	// virtual address of the segment in memory
	uint64_t	p_paddr;	// on systems where physical address is relevant, reserved for segment's physical address.
	uint64_t	p_filesz;	// size in bytes of the segment in the file image
	uint64_t	p_memsz;	// size in bytes of the segment in memory.
	uint64_t	p_align;	// 0 and 1 specify no alignment, else a integral power of 2, with p_vaddr equating p_offset modulus p_align
} __attribute__((packed));

struct ELF64SectionHeader {
    uint32_t	sh_name;		// offset to a string in the .shstrtab section that represents the name of this section
    uint32_t	sh_type;		// type of heeader (ELFSectionHeaderType)
    uint64_t	sh_flags;		// identifies the attributes of the section
    uint64_t	sh_addr;		// virtual address of the section in memory, for sections that are loaded
    uint64_t 	sh_offset;		// offset of the section in the file image
    uint64_t	sh_size;		// size in bytes of the section in the file image
    uint32_t	sh_link;		// contains the section index of an associated section
    uint32_t	sh_info;		// extra info
    uint64_t	sh_addralign;	// contains the required alignment of the section. This field must be a power of two
    uint64_t	sh_entsize;		// size, in bytes, of each entry, for sections that contain fixed-size entries else 0
} __attribute__((packed));

struct ELF64Symbol {
	uint32_t	st_name;
	uint8_t		st_info;
	uint8_t		st_other;
	uint16_t	st_shndx;
	uint64_t	st_value;
	uint64_t	st_size;
} __attribute__((packed));