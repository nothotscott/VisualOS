/*
 * File:		idt.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <memory.h>
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "cpu/gdt.h"
#include "idt.h"


struct IDTEntry* g_idt;
struct IDTDescriptor g_idt_descriptor;
void (*g_isr_handlers[ISR_MAX])(struct InterruptStack*, size_t);


void idt_init() {
	size_t idt_total_size = IDT_SIZE * sizeof(struct IDTEntry);	// size of all IDT enteries in bytes
	size_t pages = MEMORY_PAGE_SIZE / idt_total_size;			// pages needed to accomodate all the idt entries
	g_idt = (struct IDTEntry*)pageframe_request();
	memset_byte(g_idt, 0, idt_total_size);
	g_idt_descriptor.limit = idt_total_size - 1;
	g_idt_descriptor.base = g_idt;
	// Sets isrs from assembly
	idt_set_isr(14, isr14, IDT_TYPE_GATE_TRAP);
	idt_set_isr(33, isr33, IDT_TYPE_GATE_INTERRUPT);
}

void idt_set_isr(size_t index, void* isr_ptr, enum IDTGateType gate) {
	g_idt[index].zero = 0;
	g_idt[index].offset_low = (ushort_t)(((ulong_t)isr_ptr & 0x000000000000ffff));
	g_idt[index].offset_mid = (ushort_t)(((ulong_t)isr_ptr & 0x00000000ffff0000) >> 16);
	g_idt[index].offset_high = (uint_t)(((ulong_t)isr_ptr & 0xffffffff00000000) >> 32);
	g_idt[index].ist = 0;
	g_idt[index].selector = 1 * sizeof(struct GDTEntry);	// code segment at index 1
	g_idt[index].type_attr = IDT_TYPE_PRESENT << 7 | IDT_TYPE_PRIVILEGE << 5 | gate;
}

void idt_register_isr_handler(size_t num, void (*handler)(struct InterruptStack*, size_t)) {
	g_isr_handlers[num] = handler;
}