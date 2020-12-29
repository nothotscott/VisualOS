/*
 * File:		idt.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "memory/pageframe.h"
#include "idt.h"
#include "memory/paging.h"

struct IDTEntry* g_idt;
//struct IDTEntry g_idt[IDT_SIZE] = {};
struct IDTDescriptor g_idt_descriptor;/* = {
	.limit = 4095,
	.base = (struct IDTEntry*)&g_idt
};*/


void idt_init() {
	size_t idt_total_size = IDT_SIZE * sizeof(struct IDTEntry);	// size of all IDT enteries in bytes
	size_t pages = MEMORY_PAGE_SIZE / idt_total_size;			// pages needed to accomodate all the idt entries
	// TODO handle a case where pages is not 1
	g_idt = (struct IDTEntry*)pageframe_request();
	// TODO memset
	for(void* ptr = g_idt; ptr < (void*)g_idt + idt_total_size; ptr++) {
		*(byte_t*)ptr = 0;
	}
	g_idt_descriptor.limit = idt_total_size - 1;
	g_idt_descriptor.base = g_idt;
}

void idt_set_isr(size_t offset, ulong_t isr_ptr) {
	g_idt[offset].zero = 0;
	g_idt[offset].offset_low = (ushort_t)(((ulong_t)isr_ptr & 0x000000000000ffff));
	g_idt[offset].offset_mid = (ushort_t)(((ulong_t)isr_ptr & 0x00000000ffff0000) >> 16);
	g_idt[offset].offset_high = (uint_t)(((ulong_t)isr_ptr & 0xffffffff00000000) >> 32);
	g_idt[offset].ist = 0;
	g_idt[offset].selector = IDT_SELECTOR_DEFAULT;
	g_idt[offset].types_attr = IDT_TYPES_ATTR_DEFAULT;
}
