#include "io.h"


void outb(ushort_t port, byte_t val) {
	asm volatile ("outb %0, %1"
				:
				: "a"(val), "Nd"(port));
}

byte_t inb(ushort_t port) {
	byte_t ret;
	asm volatile ("inb %1, %0"
				: "=a"(ret)
				: "Nd"(port));
	return ret;
}

void io_wait() {
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
    /* %%al instead of %0 makes no difference.  TODO: does the register need to be zeroed? */
}
