/**
 * Stdlib functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <stdint.h>

void str(int, char[]);
void str(double, char[], int precision = 15);
void str(float, char[], int precision = 7);
uint64_t strol(const char *, uint8_t);
void __attribute__((no_caller_saved_registers)) hstr(unsigned int, char[]);
