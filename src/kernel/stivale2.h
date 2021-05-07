/*
 * File:		stivale2.h
 * Description:	stivale2 boot protocol headers
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "framebuffer.h"
#include "memory/memory.h"
#include "module.h"

#define STIVALE2_STACK_PAGES	1

// *** Miscellaneous datatypes *** //

enum Stivale2HeaderTagIdentifier {
	STIVALE2_HEADER_TAG_IDENTIFIER_FRAMEBUFFER		= 0x3ecc1bc43d0f7971,
	STIVALE2_HEADER_TAG_IDENTIFIER_MTRR_WC			= 0x4c7bb07731282e00,
	STIVALE2_HEADER_TAG_IDENTIFIER_TERMINAL			= 0xa85d499b1823be72,
	STIVALE2_HEADER_TAG_IDENTIFIER_5LEVEL_PAGING	= 0x932f477032007e8f,
	STIVALE2_HEADER_TAG_IDENTIFIER_UNMAP_NULL		= 0x92919432b16fe7e7,
	STIVALE2_HEADER_TAG_IDENTIFIER_SMP				= 0x1ab015085f3273df,
};
enum Stivale2StructureTagIdentifier {
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_COMMANDLINE		= 0xe5e76a1b4597a781,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_MEMORYMAP			= 0x2187f79e8612de07,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_FRAMEBUFFER		= 0x506461d2950408fa,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_EDID				= 0x968609d7af96b845,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_MTRR_WC			= 0x6bc1a78ebe871172,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_TERMINAL			= 0xc2b3f4c3233b0974,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_MODULES			= 0x4b6fe466aade04ce,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_RSDP				= 0x9e1786930a375e78,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_SMBIOS			= 0x274bd246c62bf7d1,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_EPOCH				= 0x566a7bed888e1407,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_FIRMWARE			= 0x359d837855e3858c,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_EFI_SYSTEM_TABLE	= 0x4bc5ec15845b558e,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_KERNEL_FILE		= 0xe599d90c2975584a,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_KERNEL_SLIDE		= 0xee80847d01506c57,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_SMP				= 0x34d1d96339647025,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_PXE_SERVER		= 0x29d1e96239247032,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_MMIO32_UART		= 0xb813f9b8dbc78797,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_DEVICE_TREE		= 0xabb29bd49a2833fa,
	STIVALE2_STRUCTURE_TAG_IDENTIFIER_MMIO32_VMAP		= 0xb0ed257db18cb58f,
};

enum Stivale2MemoryType {
	STIVALE2_MEMORY_TYPE_USABLE					= 1,
	STIVALE2_MEMORY_TYPE_RESERVED				= 2,
	STIVALE2_MEMORY_TYPE_ACPI_RECLAIMABLE		= 3,
	STIVALE2_MEMORY_TYPE_ACPI_NVS				= 4,
	STIVALE2_MEMORY_TYPE_BAD_MEMORY				= 5,
	STIVALE2_MEMORY_TYPE_BOOTLOADER_RECLAIMABLE	= 0x1000,
	STIVALE2_MEMORY_TYPE_KERNEL_AND_MODULES		= 0x1001,
	STIVALE2_MEMORY_TYPE_FRAMEBUFFER			= 0x1002
};

struct Stivale2MemoryMapEntry {
	uint64_t	base;
	uint64_t	length;
	uint32_t	type;
	uint32_t	unused;
} __attribute__((packed));

struct Stivale2Module {
	uint64_t	begin;			// physical address module start
	uint64_t	end;			// physical address module end
	char		string[128];	// null terminated optional string
} __attribute__((packed));

struct Stivale2SMPInfo {
	uint32_t	acpi_processor_uid;
	uint32_t	lapic_id;
	uint64_t	target_stack;
	uint64_t	goto_address;
	uint64_t	extra_argument;
} __attribute__((packed));


// *** Headers (give to bootloader) *** //

struct Stivale2Header {
	uint64_t	entry_point;
	uint64_t	stack;
	uint64_t	flags;
	uint64_t	tags;
} __attribute__((packed));

struct Stivale2HeaderTag {
	enum Stivale2HeaderTagIdentifier	identifier;
	uint64_t							next;
} __attribute__((packed));

struct Stivale2HeaderTagFrameBuffer {
	struct Stivale2HeaderTag	tag;
	// If all values are set to 0 then the bootloader will pick the best possible video mode automatically.
	uint16_t					width;
	uint16_t					height;
	uint16_t					bpp;
} __attribute__((packed));

struct Stivale2HeaderTagTerminal {
	struct Stivale2HeaderTag	tag;
	uint64_t					flags;
} __attribute__((packed));

// *** Structures (received from bootloader) *** //

struct Stivale2Structure {
	char		bootloader_brand[64];
	char		bootloader_version[64];
	uint64_t	tags;
} __attribute__((packed));

struct Stivale2StructureTag {
	enum Stivale2StructureTagIdentifier	identifier;
	uint64_t							next;
} __attribute__((packed));

struct Stivale2StructureTagCommandLine {
	struct Stivale2StructureTag	tag;
	uint64_t					cmdline;
} __attribute__((packed));

struct Stivale2StructureTagMemoryMap {
	struct Stivale2StructureTag		tag;
	uint64_t						entries_num;
	struct Stivale2MemoryMapEntry	entries[];
} __attribute__((packed));

struct Stivale2StructureTagFramebuffer {
	struct Stivale2StructureTag	tag;
	uint64_t					base;
	uint16_t					width;
	uint16_t					height;
	uint16_t					pitch;
	uint16_t					bpp;
	uint8_t						memory_model;		// 1 = RGB
	uint8_t						red_mask_size;
	uint8_t						red_mask_shift;
	uint8_t						green_mask_size;
	uint8_t						green_mask_shift;
	uint8_t						blue_mask_size;
	uint8_t						blue_mask_shift;
} __attribute__((packed));

struct Stivale2StructureTagEDID {
	struct Stivale2StructureTag	tag;
	uint64_t					info_size;
	uint8_t						info[];
} __attribute__((packed));

struct Stivale2StructureTagTerminal {
	struct Stivale2StructureTag	tag;
	uint32_t					flags;		// bit 0 = has cols and rows
	uint16_t					cols;
	uint16_t					rows;
	uint64_t					term_write;	// pointer to buffer
} __attribute__((packed));

struct Stivale2StructureModules {
	struct Stivale2StructureTag	tag;
    uint64_t					modules_num;
    struct Stivale2Module		modules[];
} __attribute__((packed));

struct Stivale2StructureRSDP {
	struct Stivale2StructureTag	tag;
	uint64_t					rsdp;
} __attribute__((packed));

struct Stivale2StructureSMBIOS {
	struct Stivale2StructureTag	tag;
	uint64_t					flags;
	uint64_t					smbios_entry_32;
	uint64_t					smbios_entry_64;
} __attribute__((packed));

struct Stivale2StructureEpoch {
	struct Stivale2StructureTag	tag;
	uint64_t					epoch;
} __attribute__((packed));

struct Stivale2StructureFirmware {
	struct Stivale2StructureTag	tag;
	uint64_t					flags;	// bit 0: 0 = UEFI, 1 = BIOS
} __attribute__((packed));

struct Stivale2StructureEFISystemTable {
	struct Stivale2StructureTag	tag;
	uint64_t					system_table;
} __attribute__((packed));

struct Stivale2StructureKernelFile {
	struct Stivale2StructureTag	tag;
	uint64_t					kernel_file;
} __attribute__((packed));

struct Stivale2StructureKernelSlide {
	struct Stivale2StructureTag	tag;
	uint64_t					kernel_slide;
} __attribute__((packed));

struct Stivale2StructureSMP {
	struct Stivale2StructureTag	tag;
	uint64_t					flags;
	uint32_t					bsp_local_apic_id;
	uint32_t					unused;
	uint64_t					cpu_count;
	struct Stivale2SMPInfo		info[];
} __attribute__((packed));

struct Stivale2StructurePXEServer {
	struct Stivale2StructureTag	tag;
	uint32_t					server_ip;
} __attribute__((packed));

struct Stivale2StructureMMIO32UART {
	struct Stivale2StructureTag	tag;
	uint64_t					address;
} __attribute__((packed));

struct Stivale2StructureDeviceTree {
	struct Stivale2StructureTag	tag;
	uint64_t					address;
	uint64_t					size;
} __attribute__((packed));

struct Stivale2StructureVMap {
	struct Stivale2StructureTag	tag;
	uint64_t					address;	// VMAP_HIGH, where the physical memory is mapped in the higher half
} __attribute__((packed));


// *** Functions *** //

// Initializes abstract bootloader using stivale2 
void stivale2_init(struct Stivale2Structure* structure);

// Gets the stivale2 structure for [identifier] at [structure]
struct Stivale2StructureTag* stivale2_get_structure(struct Stivale2Structure* structure, enum Stivale2StructureTagIdentifier identifier);
// Gets the stivale2 framebuffer and write the transformation to [framebuffer] for [structure]
void stivale2_get_framebuffer(struct Stivale2Structure* structure, struct Framebuffer* framebuffer);
// Gets the stivale2 memorymap and write the transformation of all entries to [memorymap] for [structure]
void stivale2_get_memorymap(struct Stivale2Structure* structure, struct MemoryMap* memorymap);
// Gets the stivale2 module with [signature] and [header_size] and writes the pointer to [module] for [structure]. Returns true on success.
bool stivale2_get_module(struct Stivale2Structure* structure, struct Module* module, const char* signature, size_t header_size);