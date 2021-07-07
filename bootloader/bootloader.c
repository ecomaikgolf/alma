#include "elf.h"
#include <uefi.h>

#define STRING_SIZE 60
 
const char* kernel_file = "kernel.elf";

uint8_t verify_elf_headers(Elf64_Ehdr *elf_header);

typedef struct {
	unsigned int* base;
	unsigned long long buffer_size;
	unsigned int width;
	unsigned int height;
	unsigned int ppscl;
} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_Header;

typedef struct {
	PSF1_Header* header;
	void* buffer;
} PSF1_Font;

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

	/* First data of ELF file is the ELF header */
	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)memory;
	/* Check the header data is correct */
	uint8_t elf_parse_flags = verify_elf_headers(elf_header);

	printf("(I) [bootloader]  verify_elf_headers() code: %d\n", elf_parse_flags);

	printf("(I) [bootloader]  starting to load ELF program headers (total: %d)\n", elf_header->e_phnum);

	int i;
	for(int i = 0 ; i < elf_header->e_phnum; i++) {
		Elf64_Phdr *prog_hdr = (Elf64_Phdr *)(memory + elf_header->e_phoff + elf_header->e_phentsize * i);

		/* if program segment is loadable */
		if(prog_hdr->p_type == PT_LOAD) {
			printf("(I) [bootloader]  loading program header %d at: 0x%p\n", i, prog_hdr->p_vaddr);

			memcpy((void *)prog_hdr->p_vaddr, memory + prog_hdr->p_offset, prog_hdr->p_filesz);
			memset((void*)(prog_hdr->p_vaddr + prog_hdr->p_filesz), 0, prog_hdr->p_memsz - prog_hdr->p_filesz);
		} else {
			printf("(I) [bootloader]  program header %d of type \n", i, prog_hdr->p_type);
		}
	}

	printf("(I) [bootloader]  loading ELF program headers finished\n", elf_header->e_phnum);

	/* Getting GOP */
	printf("(I) [bootloader]  initialising Graphics Output Protocol (GOP)\n");
	efi_gop_t *gop = NULL;
	efi_guid_t gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	efi_status_t status = BS->LocateProtocol(&gop_guid, NULL, (void**)&gop);
	if(gop == NULL || EFI_ERROR(status)) {
		printf("(E) [bootloader]  unable to locate/initialise GOP\n");
	}

	Framebuffer fb;
	fb.base 	   = (void *)gop->Mode->FrameBufferBase;
	fb.buffer_size = gop->Mode->FrameBufferSize;
	fb.width       = gop->Mode->Information->HorizontalResolution;
	fb.height      = gop->Mode->Information->VerticalResolution;
	fb.ppscl 	   = gop->Mode->Information->PixelsPerScanLine;

	printf("(I) [bootloader]  GOP base: 0x%x\n", fb.base);
	printf("(I) [bootloader]  GOP buffer size: 0x%x\n", fb.buffer_size);
	printf("(I) [bootloader]  GOP width: %d\n", fb.width);
	printf("(I) [bootloader]  GOP height: %d\n", fb.height);
	printf("(I) [bootloader]  GOP pixels per scan line: %d\n", fb.ppscl);

	/* Load PSF1 font */
	printf("(I) [bootloader]  PSF1 loading font file\n");
	FILE *font_file = fopen("zap-light16.psf", "r");
	/* Move the file pointer to the end */
	fseek(font_file, 0, SEEK_END);
	/* Return current in bytes */
	uint64_t font_size = ftell(font_file);
	/* Move the file pointer to the start */
	fseek(font_file, 0, SEEK_SET);
	void* font_buffer = malloc(font_size);
	fread((void *)font_buffer, font_size, 1, font_file);

	printf("(I) [bootloader]  PSF1 reading header\n");
	PSF1_Header *fhead = (PSF1_Header *)font_buffer;
	
	if(fhead->magic[0] != PSF1_MAGIC0 || fhead->magic[1] != PSF1_MAGIC1) {
		printf("(E) [bootloader]  PSF1 font incorrect magic header\n");
	} else {
		printf("(I) [bootloader]  PSF1 font correct magic header\n");
	}

	printf("(I) [bootloader]  PSF1 reading glyph data\n");

	/* Assume ignore unicode inf. */
	PSF1_Header *font_glyph = (font_buffer + sizeof(PSF1_Header)); 

	/* Create the font */
	PSF1_Font *font = malloc(sizeof(PSF1_Font));
	font->header = fhead;
	font->buffer = font_glyph;

	printf("(I) [bootloader]  jumping to kernel code at address: 0x%p\n", elf_header->e_entry);

	void (*_start)() = ((__attribute__((sysv_abi)) void (*)(Framebuffer*, PSF1_Font*) ) elf_header->e_entry);

	/* Call kernel */
	_start(&fb, font);

	printf("(I) [bootloader]  returned from kernel\n");

	return 0;
}

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

uint8_t
verify_elf_headers(Elf64_Ehdr *elf_header)
{
	#define EM_MACH 62 //x86_64 https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.eheader.htm
	uint8_t elf_parse_flags = 0;

	for(int i = 0 ; i < NUM_CHECKS ; i++) {
		uint8_t check = 255;
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

		if(check != 255) {
			elf_parse_flags |= check << i; 
			printf("%s\n", verifymessages[i][check]);
		}
	}

	return elf_parse_flags;
}
