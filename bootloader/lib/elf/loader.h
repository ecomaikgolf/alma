#include "elf/types.h"

uint8_t verify_elf_headers(Elf64_Ehdr *elf_header);
Elf64_Ehdr* load_file(const char* filename);
