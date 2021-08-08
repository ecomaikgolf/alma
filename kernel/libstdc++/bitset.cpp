/**
 * Bitset class
 *
 * An "array of bits"
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#include "bitset.h"

using namespace std;

/**
 * Return the bit value of index 'index'
 *
 * @param index Index to look
 * @return true -> bit 1 | false -> bit 0
 */
bool
bitset::operator[](size_t index)
{
    auto byte = static_cast<int>(index / 8);
    auto bit  = index % 8;

    uint8_t value = *(buffer + byte);

    return (value & (0b10000000 >> bit));
}

/**
 * Set a bit to true
 *
 * @param index Index of the bit
 */
void
bitset::set(size_t index)
{
    auto byte = static_cast<int>(index / 8);
    auto bit  = index % 8;

    uint8_t value    = *(buffer + byte);
    uint8_t newvalue = value | (0b10000000 >> bit);

    *(buffer + byte) = newvalue;
}

/**
 * Set a bit to false
 *
 * @param index Index of the bit
 */
void
bitset::unset(size_t index)
{
    auto byte = static_cast<int>(index / 8);
    auto bit  = index % 8;

    uint8_t value    = *(buffer + byte);
    uint8_t newvalue = value & ~(0b10000000 >> bit);

    *(buffer + byte) = newvalue;
}
