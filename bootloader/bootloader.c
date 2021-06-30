#include "elf.h"
#include <uefi.h>

int main(int argc, char *argv[]) 
{
	printf("(I) [bootloader] loading bootloader\n");

	/*if(argc < 2) {
		fprintf(stderr, "bootloader <kernel>\n");
		return 1;
	}*/

	FILE *kernel = fopen("kernel.elf", "r");

	if(kernel == NULL) {
		fprintf(stderr, "(E) [bootloader] kernel.elf not found\n", "kernel.elf");
		return 2;
	}

	/* Move the file pointer to the end */
	fseek(kernel, 0, SEEK_END);
	/* Return current in bytes */
	uint64_t kernel_size = ftell(kernel);
	/* Move the file pointer to the start */
	fseek(kernel, 0, SEEK_SET);

	/* Alloc kernel size */
	char* memory = malloc(kernel_size);

	if(memory == NULL) {
		fprintf(stderr, "(E) [bootloader] not enough memory to allocate the kernel size\n", kernel_size);
		return 3;
	}

	/* Copy file contents to memory */
	fread((void *)memory, kernel_size, 1, kernel);
	/* Close the file */
	fclose(kernel);

	printf("(I) [bootloader] kernel.elf found\n");
	return 0;
}
