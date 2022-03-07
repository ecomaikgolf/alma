#include "libc/string.h"

uint32_t
strlen(const char *str)
{
    for (uint32_t i = 0;; i++) {
        if (str[i] == '\0')
            return i;
    }
}
