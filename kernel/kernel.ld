/* https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html */

ENTRY(_start)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS 
{
	.text : ALIGN(0x1000) 
	{
		*(.text)
	}
	.data : ALIGN(0x1000) 
	{
		*(.data)
	}
	.rodata : ALIGN(0x1000) 
	{
		*(.rodata)
	}
	.bss : ALIGN(0x1000) 
	{
		/* *(COMMON) by itself refers to all uninitialized data from all input files 
		 * https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html
		 */
		*(COMMON) 
		*(.bss)
	}
}
