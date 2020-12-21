/*
 * File:		idt.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "memory/paging.h"
#include "idt.h"

struct IDTEntry* g_idt;
struct IDTDescriptor g_idt_descriptor;


void idt_init() {
	size_t idt_total_size = IDT_SIZE * sizeof(struct IDTEntry);	// size of all IDT enteries in bytes
	size_t pages = MEMORY_PAGE_SIZE / idt_total_size;			// pages needed to accomodate all the idt entries
	// TODO handle a case where pages is not 1
	g_idt = (struct IDTEntry*)page_request();
	// TODO memset
	for(void* ptr = g_idt; ptr < (void*)g_idt + idt_total_size; ptr++) {
		*(byte*)ptr = 0;
	}
	g_idt_descriptor.limit = idt_total_size - 1;
	g_idt_descriptor.base = g_idt;
}

void idt_set_isr(size_t offset, ulong isr_ptr) {
	/*struct IDTEntry* idt_entry = g_idt + offset;
	idt_entry->zero = 0;
	idt_entry->offset_low = (ushort)(((ulong)isr_ptr & 0x000000000000ffff));
	idt_entry->offset_mid = (ushort)(((ulong)isr_ptr & 0x00000000ffff0000) >> 16);
	idt_entry->offset_high = (uint)(((ulong)isr_ptr & 0xffffffff00000000) >> 32);
	idt_entry->ist = 0;
	idt_entry->selector = IDT_DEFAULT_SELECTOR;
	idt_entry->types_attr = IDT_DEFAULT_TYPES_ATTR;*/
	g_idt[offset].zero = 0;
	g_idt[offset].offset_low = (ushort)(((ulong)isr_ptr & 0x000000000000ffff));
	g_idt[offset].offset_mid = (ushort)(((ulong)isr_ptr & 0x00000000ffff0000) >> 16);
	g_idt[offset].offset_high = (uint)(((ulong)isr_ptr & 0xffffffff00000000) >> 32);
	g_idt[offset].ist = 0;
	g_idt[offset].selector = IDT_DEFAULT_SELECTOR;
	g_idt[offset].types_attr = IDT_DEFAULT_TYPES_ATTR;
}
