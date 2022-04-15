/**
 * sqrt
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "lib/math.h"

/**
 * sqrt function
 * @author: musl libc
 */
double
sqrt(double x)
{
    __asm__("sqrtsd %1, %0" : "=x"(x) : "x"(x));
    return x;
}
