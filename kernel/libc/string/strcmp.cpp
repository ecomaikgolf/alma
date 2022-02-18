#include "libc/math.h"
#include "libc/string.h"

/**
 * strcmp function
 *
 * Maybe is not the fastest one, but just_works^{TM}
 *
 * @s1 Null terminated char*
 * @s2 Null terminated char*
 *
 * @return -1 if s1 < s2, 0 if s1 == s2, 1 if s1 > s2
 */
int
strcmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' || *s2 != '\0') {
        if (*s1 != *s2)
            return (*s1 - *s2) / (abs(*s1 - *s2));
        s1++;
        s2++;
    }

    return (*s1 == *s2 ? 0 : (*s1 - *s2) / (abs(*s1 - *s2)));
}
