#include "lib/math.h"
#include "lib/string.h"

/**
 * strcmp function
 *
 * From GNU C Library
 *
 * @s1 Null terminated char*
 * @s2 Null terminated char*
 *
 * @return < 0 if s1 < s2, = 0 if s1 == s2, > 0 if s1 > s2
 */
int
strcmp(const char *s1, const char *s2)
{
    do {
        s1++;
        s2++;
        if (*s1 == '\0')
            return *s1 - *s2;

    } while (*s1 == *s2);

    return *s1 - *s2;
}

/**
 * strncmp function
 *
 * From GNU C Library
 *
 * @s1 Null terminated char*
 * @s2 Null terminated char*
 *
 * @return < 0 if s1 < s2, = 0 if s1 == s2, > 0 if s1 > s2
 */
int
strncmp(const char *s1, const char *s2, unsigned int n)
{
    while (n-- > 0) {
        if (*(s1++) != *(s2++))
            return *s1 - *s2;
        if (*s1 == '\0')
            return 0;
    }

    return 0;
}
