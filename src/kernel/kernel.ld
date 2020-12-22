OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
	_kernel_start = .;
	.text : ALIGN(4K)
	{
		*(.text)
	}
	/*.idt BLOCK(4K) : ALIGN(4K)
	{
		g_idt = ABSOLUTE(.);
		. = . + 4K;
	}*/
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
}