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
strcmp(const char *p1, const char *p2)
{
    const unsigned char *s1 = (const unsigned char *)p1;
    const unsigned char *s2 = (const unsigned char *)p2;
    unsigned char c1, c2;

    do {
        c1 = (unsigned char)*s1++;
        c2 = (unsigned char)*s2++;
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);

    return c1 - c2;
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
