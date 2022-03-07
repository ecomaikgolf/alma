#include "lib/math.h"
#include "lib/stdlib.h"
#include "lib/string.h"

uint64_t
strol(const char *str, uint8_t base)
{
    uint64_t result = 0;
    uint32_t exp    = 0;
    for (int32_t i = strlen(str) - 1; i >= 0; i--, exp++) {
        if (str[i] <= 57)
            result += (str[i] - '0') * pow(base, exp);
        else
            result += ((str[i] - 'A') + 10) * pow(base, exp);
    }
    return result;
}
