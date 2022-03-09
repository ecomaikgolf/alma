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

/**
 * Convert hex number to string (provided buffer)
 * @post Result is in result[] buffer parameter
 *
 * @param num Number to convert
 * @param result Provided buffer and result
 */
template<typename T>
void
hstr(T num, char result[])
{
    /** Pointer at the end of the number */
    unsigned char *ptr = reinterpret_cast<unsigned char *>(&num + 1);
    ptr--;

    /** Iterate over the bytes */
    for (unsigned long i = 0; i < sizeof(T); i++) {
        /** 1 byte = 2 hex values */
        int lval           = (*ptr & 0xF0) >> 4;
        int rval           = (*ptr & 0x0F);
        constexpr int skip = ('`' - 9);
        result[2 * i]      = lval + (lval > 9 ? skip : '0');
        result[2 * i + 1]  = rval + (rval > 9 ? skip : '0');
        ptr--;
    }

    result[2 * sizeof(T)] = '\0';
}
