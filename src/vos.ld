OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kernel_start_default)


SECTIONS {
	_virtual_base = 0xffffffff80000000;
	. = _virtual_base + 2M;
	_kernel_start = .;
	
	.stivale2hdr : {
		KEEP(*(.stivale2hdr))
	}
	.text : ALIGN(4K)
	{
		*(.text)
		*(.text.rel) *(.text.rel.*)
	}
	.rodata : ALIGN(4K)
	{
		*(.rodata .rodata.*)
	}

	_kernel_writable_start = .;
	.bss : ALIGN(4K)
	{
		*(COMMON)
		*(.bss .bss.*)
		*(.bss.rel .bss.rel.*)
	}
	.data : ALIGN(4K)
	{
		*(.data .data.*)
	}
	_kernel_writable_end = .;

	_kernel_end = .;
	/DISCARD/ : { *(.eh_frame) }
}