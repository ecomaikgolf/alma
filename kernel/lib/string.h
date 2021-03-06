/**
 * String functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <stdint.h>

void memset(void *, uint8_t, uint64_t);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, unsigned int);
uint32_t strlen(const char *);
