#include "bitset.h"

using namespace std;

bool
bitset::operator[](size_t index)
{
    auto byte = static_cast<int>(index / 8);
    auto bit  = index % 8;

    uint8_t value = *(buffer + byte);

    return (value & (0b10000000 >> bit));
}

void
bitset::set(size_t index)
{
    auto byte = static_cast<int>(index / 8);
    auto bit  = index % 8;

    uint8_t value    = *(buffer + byte);
    uint8_t newvalue = value | (0b10000000 >> bit);

	*(buffer + byte) = newvalue;
}

void
bitset::unset(size_t index)
{
    auto byte = static_cast<int>(index / 8);
    auto bit  = index % 8;

    uint8_t value    = *(buffer + byte);
    uint8_t newvalue = value & ~(0b10000000 >> bit);

	*(buffer + byte) = newvalue;
}
