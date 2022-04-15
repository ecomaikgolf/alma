/**
 * pow
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "lib/math.h"

/**
 * pow function
 */
unsigned int
pow(unsigned int base, unsigned int exponent)
{
    unsigned int res = 1;

    while (exponent--) {
        res *= base;
    }

    return res;
}
