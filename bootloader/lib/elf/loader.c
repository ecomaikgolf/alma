/**
 * ELF loading functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "elf/loader.h"
#include "io/file.h"
#include "log/stdout.h"

/** Machine type
 *
 * Defined as x86_64, our target
 * https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.eheader.html
 */
#define EM_MACH 62

/** Number of checks in the verify_elf_headers loop */
const int NUM_CHECKS = 5;

// clang-format off
/** Info check messages */
char verifymessages[][60] = {
    "ELF magic number is correct\0",
    "ELF is a executable object",
    "ELF target arch is x86_64",
    "ELF target is 64 bits",
    "ELF program header counter is non zero",
};

/** Error check messages */
char errormessages[][60] = {
    "ELF magic number is incorrect\0",
	"ELF is not a executable object",
    "ELF target arch is not x86_64",
	"ELF target is not 64 bits",
    "ELF program header counter is zero",
};
// clang-format on

/**
 * Load ELF file from a memory chunk
 *
 * Parses, verifies, does the memory management, etc.
 */
Elf64_Ehdr *
load_elf(const char *const filename)
{
    char *memory = (char *)load_file(filename);

    if(memory == NULL)
        return NULL;

    Elf64_Ehdr *header = get_elf_header(memory);
    uint8_t flags      = verify_elf_headers(header);

    if (flags > 0) {
        error("wrong ELF header, status code: %d", flags);
        return NULL;
    }

    load_phdrs(header, memory);

    return header;
}

/**
 * Get the ELF header from a memory chunk
 *
 * @info ELF specification states that the ELF header has to be the first thing
 */
Elf64_Ehdr *
get_elf_header(char *memory)
{
    /* First data of ELF file is the ELF header */
    return (Elf64_Ehdr *)memory;
}

/**
 * Verify ELF header
 *
 * Performs switch defined checks to see if the header is correct and good to load
 *
 * To add more checks:
 * 	- Add a new case. Mark check 0 if correct, 1 if not correct
 * 	- Append a new info string and error string in the header
 * 	- Increase NUM_CHECKS in the header by one
 * 	- Max 8 checks, increase return int size to add more flags
 */
uint8_t
verify_elf_headers(const Elf64_Ehdr *const elf_header)
{
    if (elf_header == NULL) {
        warning("elf_header parameter is null");
        return 255;
    }

    uint8_t elf_parse_flags = 0;

    for (int i = 0; i < NUM_CHECKS; i++) {
        uint8_t check = 255; /* NUM_CHECKS > actual checks, throw warning */
        switch (i) {
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

        if (check != 255) {
            /* Mark the bit with 1 to identify the check error */
            elf_parse_flags |= check << i;

            (check > 0) ? error("%s", errormessages[i]) : info("%s", verifymessages[i]);
        }
    }

    return elf_parse_flags;
}

/**
 * Load ELF program segments
 *
 * @post Loaded program headers are in memory
 */
void
load_phdrs(const Elf64_Ehdr *const elf_header, const char *const memory)
{
    for (int i = 0; i < elf_header->e_phnum; i++) {
        Elf64_Phdr *prog_hdr =
          (Elf64_Phdr *)(memory + elf_header->e_phoff + elf_header->e_phentsize * i);
        switch (prog_hdr->p_type) {
            /* Loadable */
            case PT_LOAD: {
                info("loading program header %d at: 0x%p", i, prog_hdr->p_vaddr);

                memcpy((void *)prog_hdr->p_vaddr, memory + prog_hdr->p_offset, prog_hdr->p_filesz);
                memset((void *)(prog_hdr->p_vaddr + prog_hdr->p_filesz),
                       0,
                       prog_hdr->p_memsz - prog_hdr->p_filesz);
                break;
            }
            /* Ignore */
            default:
                info("program header %d of type %d ignored", i, prog_hdr->p_type);
        }
    }
}
