OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

/*PHDRS
{
	headers PT_PHDR PHDRS;
	text PT_LOAD FILEHDR PHDRS;
}*/


SECTIONS
{
	. = SIZEOF_HEADERS;
	_kernel_start = .;
	.text : ALIGN(4K)
	{
		*(.text)
	}/* :text */
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