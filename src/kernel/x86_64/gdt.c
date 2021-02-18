/*
 * File:		gdt.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include "memory/pageframe.h"
#include "gdt.h"


struct GDTEntry* g_gdt;
struct GDTDescriptor g_gdt_descriptor;
struct TSS* g_tss;
struct StackTable* g_ring_stacks[GDT_TSS_RING_STACKS_NUM];
struct StackTable* g_ist_stacks[GDT_TSS_IST_STACKS_NUM];


void gdt_init() {
	uint16_t gdt_total_size = GDT_SIZE * sizeof(struct GDTEntry);
	g_gdt = (struct GDTEntry*)pageframe_request();
	g_tss = (struct TSS*)pageframe_request();
	// Setup stacks
	for(size_t i = 0; i < GDT_TSS_RING_STACKS_NUM; i++) {
		g_ring_stacks[i] = (struct StackTable*)pageframe_request();
	}
	for(size_t i = 0; i < GDT_TSS_IST_STACKS_NUM; i++) {
		g_ist_stacks[i] = (struct StackTable*)pageframe_request();
	}
	// Set entries
	memset(g_gdt, 0, gdt_total_size);	// implictly define null descriptor
	gdt_set_entry(1, true);				// code segment
	gdt_set_entry(2, false);			// data segment
	gdt_set_tss(3);						// task state segment
	// Set descriptor
	g_gdt_descriptor.size = gdt_total_size - 1;
	g_gdt_descriptor.offset = g_gdt;
}

void gdt_set_entry(size_t index, bool is_code) {
	memset(g_gdt + index, 0, sizeof(struct GDTEntry));
	g_gdt[index].access |= 0b00000010;	// Writable
	g_gdt[index].access |= 0b00010000;	// Type
	g_gdt[index].access |= 0b10000000;	// Present
	g_gdt[index].access |= is_code ? 0b00001000 : 0b0;	// Executable
	g_gdt[index].flags |= is_code ? 0b00100000 : 0b0;	// Long-mode
}

void gdt_set_tss(size_t index) {
	g_tss->io_map_base = sizeof(struct TSS);
	// Ring stacks
	g_tss->rsp0_low = (uint64_t)g_ring_stacks[0] & 0xffffffff;
	g_tss->rsp0_high = ((uint64_t)g_ring_stacks[0] >> 32) & 0xffffffff;
	g_tss->rsp1_low = (uint64_t)g_ring_stacks[1] & 0xffffffff;
	g_tss->rsp1_high = ((uint64_t)g_ring_stacks[1] >> 32) & 0xffffffff;
	g_tss->rsp2_low = (uint64_t)g_ring_stacks[2] & 0xffffffff;
	g_tss->rsp2_high = ((uint64_t)g_ring_stacks[2] >> 32) & 0xffffffff;
	// Interrupt stacks
	g_tss->ist1_low = (uint64_t)g_ist_stacks[0] & 0xffffffff;
	g_tss->ist1_high = ((uint64_t)g_ist_stacks[0] >> 32) & 0xffffffff;
	g_tss->ist2_low = (uint64_t)g_ist_stacks[1] & 0xffffffff;
	g_tss->ist2_high = ((uint64_t)g_ist_stacks[1] >> 32) & 0xffffffff;
	g_tss->ist3_low = (uint64_t)g_ist_stacks[2] & 0xffffffff;
	g_tss->ist3_high = ((uint64_t)g_ist_stacks[2] >> 32) & 0xffffffff;
	g_tss->ist4_low = (uint64_t)g_ist_stacks[3] & 0xffffffff;
	g_tss->ist4_high = ((uint64_t)g_ist_stacks[3] >> 32) & 0xffffffff;
	g_tss->ist5_low = (uint64_t)g_ist_stacks[4] & 0xffffffff;
	g_tss->ist5_high = ((uint64_t)g_ist_stacks[4] >> 32) & 0xffffffff;
	g_tss->ist6_low = (uint64_t)g_ist_stacks[5] & 0xffffffff;
	g_tss->ist6_high = ((uint64_t)g_ist_stacks[5] >> 32) & 0xffffffff;
	g_tss->ist7_low = (uint64_t)g_ist_stacks[6] & 0xffffffff;
	g_tss->ist7_high = ((uint64_t)g_ist_stacks[6] >> 32) & 0xffffffff;
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