OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kernel_start_default)


SECTIONS
{
	. = 2M;
	_kernel_start = .;

    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    }

	.text : ALIGN(4K)
	{
		*(.text)
	}
	.data : ALIGN(4K)
	{
		*(.data)
	}
	.rodata : ALIGN(4K)
	{
		*(.rodata)
	}
	.bss : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
	_kernel_end = .;
	/DISCARD/ : { *(.eh_frame) }
}