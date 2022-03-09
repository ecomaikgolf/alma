/**
 * String conversion functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "lib/math.h"
#include <stdint.h>

/**
 * Convert integer to string (provided buffer)
 *
 * @post Result is in result[] buffer parameter
 *
 * @param num Number to convert
 * @param result Provided buffer and result
 */
void
str(int num, char result[])
{
    bool negative = false;
    if (num < 0) {
        result[0] = '-';
        num       = abs(num);
        negative  = true;
    }

    int positions                = int_log10(num) + 1;
    result[positions + negative] = '\0';

    for (int i = positions + negative - 1; i >= negative; i--) {
        result[i] = '0' + (num % 10);
        num /= 10;
    }
};

/**
 * Convert float to string (provided buffer)
 *
 * @post Result is in result[] buffer parameter
 *
 * @param num Number to convert
 * @param result Provided buffer and result
 * @param precision Number of decimal digits (default 15)
 */
void
str(double num, char result[], int precision)
{
    bool negative = false;
    if (num < 0) {
        result[0] = '-';
        num       = abs(num);
        negative  = true;
    }

    int integer     = static_cast<int>(num);
    double exponent = num - integer;

    int positions_integer = int_log10(num) + 1;

    for (int i = positions_integer + negative - 1; i >= negative; i--) {
        result[i] = '0' + (integer % 10);
        integer /= 10;
    }

    result[positions_integer + negative] = '.';

    int i = positions_integer + negative + 1;

    while (precision > 0) {
        exponent *= 10;
        result[i] = '0' + static_cast<int>(exponent);
        exponent -= static_cast<int>(exponent);
        precision--;
        i++;
    }
    result[i] = '\0';
};

/**
 * Convert float to string (provided buffer)
 *
 * @post Result is in result[] buffer parameter
 *
 * @param num Number to convert
 * @param result Provided buffer and result
 * @param precision Number of decimal digits (default 7)
 */
void
str(float num, char result[], int precision)
{
    str(static_cast<double>(num), result, precision);
}

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
        int lval          = (*ptr & 0xF0) >> 4;
        int rval          = (*ptr & 0x0F);
        result[2 * i]     = lval + (lval > 9 ? '7' : '0');
        result[2 * i + 1] = rval + (rval > 9 ? '7' : '0');
        ptr--;
    }

    result[2 * sizeof(T)] = '\0';
}
