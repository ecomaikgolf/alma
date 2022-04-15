/**
 * strlen
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "lib/string.h"

/**
 * strlen
 *
 * Obtain lenght of a null terminated string
 *
 * @arg str string
 * @return lenght
 */
uint32_t
strlen(const char *str)
{
    for (uint32_t i = 0;; i++) {
        if (str[i] == '\0')
            return i;
    }
}
