OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
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
	.bss : ALIGN(4K) /* Uninitalized */
	{
		*(COMMON)
		*(.bss)
	}
}