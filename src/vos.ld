OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kernel_start_default)


SECTIONS {
	. = 2M;
	_kernel_start = .;

    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    }

	.text : ALIGN(4K)
	{
		*(.text)
		*(.text.rel) *(.text.rel.*)
	}
	.data :
	{
		*(.data .data.*) *(.rodata .rodata.*)
	}
	.bss : ALIGN(4K)
	{
		*(COMMON)
		*(.bss .bss.*)
		*(.bss.rel .bss.rel.*)
	}
	_kernel_end = .;
	/DISCARD/ : { *(.eh_frame) }
}