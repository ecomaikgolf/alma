/**
 * Stdlib functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

void str(int, char[]);
void str(double, char[], int precision = 15);
void str(float, char[], int precision = 7);
void __attribute__((no_caller_saved_registers)) hstr(unsigned int, char[]);
