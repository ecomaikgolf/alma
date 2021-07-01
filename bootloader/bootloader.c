#include "elf.h"
#include <uefi.h>

#define STRING_SIZE 60
 
const char* kernel_file = "kernel.elf";

/* Cannot be const char* const due to enviromenet restrictions */
#define NUM_CHECKS 5
static char verifymessages[NUM_CHECKS][2][STRING_SIZE] = {
	{
		"(I) [bootloader]  ELF magic number is correct",
		"(E) [bootloader]  ELF magic number is incorrect",
	},
	{
		"(I) [bootloader]  ELF is a executable object",
		"(E) [bootloader]  ELF is not a executable object",
	},
	{
		"(I) [bootloader]  ELF target arch is x86_64",
		"(E) [bootloader]  ELF target arch is not x86_64",
	},
	{
		"(I) [bootloader]  ELF target is 64 bits",
		"(E) [bootloader]  ELF target is not 64 bits",
	},
	{
		"(I) [bootloader]  ELF program header counter is non zero",
		"(E) [bootloader]  ELF program header counter is zero", 
	}
};

#define resolution(i, check) 	   			\
	elf_parse_flags |= check << i; 			\
	printf("%s\n", verifymessages[i][check]);

uint8_t
verify_elf_headers(Elf64_Ehdr *elf_header)
{
	#define EM_MACH 62 //x86_64 https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.eheader.htm
	uint8_t elf_parse_flags = 0;

	for(int i = 0 ; i < NUM_CHECKS ; i++) {
		uint8_t check = -1;
		switch(i) {
			case 0:
				/* Magic header */
				check = !(memcmp(elf_header->e_ident, ELFMAG, SELFMAG) == 0);
				break;
			case 1:
				/* Executable ELF */
				check = !(elf_header->e_type == ET_EXEC);
				break;
			case 2:
				/* ELF Architecture */
				check = !(elf_header->e_machine == EM_MACH);
				break;
			case 3:
				/* ELF 64 bits target */
				check = !(elf_header->e_ident[EI_CLASS] == ELFCLASS64);
				break;
			case 4:
				/* ELF non empty program headers  */
				check = !(elf_header->e_phnum > 0);
				break;
			default: 
				printf("(W) [bootloader]  verify_elf_headers(...) default switch case reached\n");
		}

		if(check != -1)
			resolution(i, check);
	}

	return elf_parse_flags;
}

int
main(int argc, char *argv[]) 
{
	printf("(I) [bootloader]  loading bootloader\n");

	/*if(argc < 2) {
		fprintf(stderr, "bootloader <kernel>\n");
		return 1;
	}*/

	FILE *kernel = fopen(kernel_file, "r");

	if(kernel == NULL) {
		printf("(E) [bootloader]  %s not found\n", kernel_file);
		return 2;
	}

	printf("(I) [bootloader]  %s found\n", kernel_file);

	/* Move the file pointer to the end */
	fseek(kernel, 0, SEEK_END);
	/* Return current in bytes */
	uint64_t kernel_size = ftell(kernel);
	/* Move the file pointer to the start */
	fseek(kernel, 0, SEEK_SET);

	/* Alloc kernel size */
	printf("(I) [bootloader]  allocating memory for the kernel\n");
	char* memory = malloc(kernel_size);

	if(memory == NULL) {
		printf("(E) [bootloader]  not enough memory to allocate the kernel size\n");
		return 3;
	}

	/* Copy file contents to memory */
	fread((void *)memory, kernel_size, 1, kernel);
	/* Close the file */
	fclose(kernel);

	printf("(I) [bootloader]  parsing ELF data\n");

	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)memory;

	uint8_t elf_parse_flags = verify_elf_headers(elf_header);

	printf("(I) [bootloader]  verify_elf_headers() code: %d\n", elf_parse_flags);


	return 0;
}
