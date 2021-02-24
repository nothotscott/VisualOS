/*
 * File:		gdt.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "gdt.h"


struct GDTEntry* g_gdt;
struct GDTDescriptor g_gdt_descriptor;
struct TSS* g_tss;


void gdt_init() {
	uint16_t gdt_total_size = GDT_SIZE * sizeof(struct GDTEntry);
	g_gdt = (struct GDTEntry*)pageframe_request();
	g_tss = (struct TSS*)pageframe_request();
	// Set entries
	memset(g_gdt, 0, gdt_total_size);								// implictly define null descriptor
	gdt_set_entry(1, GDT_ACCESS_EXECUTABLE, 0);						// kernel code segment
	gdt_set_entry(2, 0, 0);											// kernel data segment
	memset(g_gdt + 3, 0, gdt_total_size);
	gdt_set_entry(4, GDT_ACCESS_DPL, 0);							// user data segment
	gdt_set_entry(5, GDT_ACCESS_DPL | GDT_ACCESS_EXECUTABLE, 0);	// user code segment
	gdt_set_tss(6);													// task state segment
	// Set descriptor
	g_gdt_descriptor.size = gdt_total_size - 1;
	g_gdt_descriptor.offset = g_gdt;
}

void gdt_set_entry(size_t index, enum GDTAccess access, enum GDTFlags flags) {
	memset(g_gdt + index, 0, sizeof(struct GDTEntry));
	g_gdt[index].limit_low = 0xffff;
	g_gdt[index].access = GDT_ACCESS_WRITABLE | GDT_ACCESS_TYPE | GDT_ACCESS_PRESENT | access;
	g_gdt[index].flags = GDT_FLAG_LONG_MODE | GDT_FLAG_PAGE_GRANULARITY | flags;
}

void gdt_set_tss(size_t index) {
	memset(g_tss, 0, sizeof(struct TSS));
	g_tss->io_map_base = sizeof(struct TSS);
	// Set TSS descriptor entry
	struct TSSDescriptor* tss_descriptor = (struct TSSDescriptor*)(g_gdt + index);
	memset(tss_descriptor, 0, sizeof(struct TSSDescriptor));
	tss_descriptor->limit_low |= sizeof(struct TSS);
	tss_descriptor->base_low |= (uint64_t)g_tss & 0xffff;
	tss_descriptor->base_mid |= ((uint64_t)g_tss >> 16) & 0xff;
	tss_descriptor->base_mid2 |= ((uint64_t)g_tss >> 24) & 0xff;
	tss_descriptor->base_high |= ((uint64_t)g_tss >> 32) & 0xffffffff;
	tss_descriptor->access |= 0b10000000;	// Present
	tss_descriptor->access |= 0b00001001;	// Type (Intel Manual 3A 3.4.5.1) Execute-Only, accessed
	tss_descriptor->flags |= 0b00010000;	// Available
}

void gdt_set_ring0_stack(void* stack) {
	g_tss->rsp0 = (uint64_t)stack;
}

void* gdt_get_ring0_stack(){
	return (void*)g_tss->rsp0;
}