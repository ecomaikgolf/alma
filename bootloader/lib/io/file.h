/**
 * Functions to manage files in a UEFI enviroment using posix-uefi wrappers
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <uefi.h>

uint64_t file_size(FILE *file);
void *load_file(const char *const filename);
