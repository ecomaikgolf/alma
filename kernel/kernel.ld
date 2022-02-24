/* https://home.cs.colorado.edu/~main/cs1300/doc/gnu/ld_3.html */

ENTRY(_start)
OUTPUT_FORMAT(elf64-x86-64)

/*
 * VMA and file offsets for the SYSTEM V architecture segments
 * are congruent modulo 4KB (0x1000) or larger powers of 2.
 * - ELF cs.cmu.edu guide (Portable formats specification)
 */

SECTIONS 
{
	. = 0x1000000;
	_start_addr = .;
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
		/* *(COMMON) by itself refers to all uninitialized data from all input files 
		 * https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html
		 */
		*(COMMON) 
		*(.bss)
	}
	_end_addr = .;
}
