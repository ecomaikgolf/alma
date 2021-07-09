/**
 * ELF loading functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "elf/types.h"

Elf64_Ehdr *load_elf(char *memory);
Elf64_Ehdr *get_elf_header(char *memory);
uint8_t verify_elf_headers(Elf64_Ehdr *elf_header);
void load_phdrs(Elf64_Ehdr *elf_header, char *memory);
