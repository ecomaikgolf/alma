/**
 * log10(n) integer result (quite bad but a hotfix)
 * https://helloacm.com/fast-integer-log10/
 */
unsigned int
int_log10(unsigned int number)
{
    return (number >= 1000000000u)  ? 9
           : (number >= 100000000u) ? 8
           : (number >= 10000000u)  ? 7
           : (number >= 1000000u)   ? 6
           : (number >= 100000u)    ? 5
           : (number >= 10000u)     ? 4
           : (number >= 1000u)      ? 3
           : (number >= 100u)       ? 2
           : (number >= 10u)        ? 1u
                                    : 0u;
}
