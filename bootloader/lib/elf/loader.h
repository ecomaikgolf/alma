/**
 * ELF loading functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include "elf/types.h"

Elf64_Ehdr *load_elf(const char *const);
Elf64_Ehdr *get_elf_header(char *);
uint8_t verify_elf_headers(const Elf64_Ehdr *const);
void load_phdrs(const Elf64_Ehdr *const, const char *const);
void call_ctors(Elf64_Ehdr *);
