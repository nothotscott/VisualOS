/*
 * File:		gdt.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <memory.h>
#include "memory/pageframe.h"
#include "gdt.h"


struct GDTEntry* g_gdt;
struct GDTDescriptor g_gdt_descriptor;


void gdt_init() {
	ushort_t gdt_total_size = GDT_SIZE * sizeof(struct GDTEntry);
	g_gdt = (struct GDTEntry*)pageframe_request();
	memset_byte(g_gdt, 0, gdt_total_size);			// implictly define null descriptor
	g_gdt_descriptor.size = gdt_total_size - 1;
	g_gdt_descriptor.offset = g_gdt;
	gdt_set_entry(1, true);							// code segment
	gdt_set_entry(2, false);						// data segment
}

void gdt_set_entry(size_t index, bool is_code) {
	g_gdt[index].limit_low = 0xffff;
	g_gdt[index].base_low = 0x0000;
	g_gdt[index].base_mid = 0x00;
	g_gdt[index].access = 0b10010010 | (is_code ? 0b00001000 : 0);
	g_gdt[index].flags = 0b10001111 | (is_code ? 0b00100000 : 0);
	g_gdt[index].base_high = 0x00;
}