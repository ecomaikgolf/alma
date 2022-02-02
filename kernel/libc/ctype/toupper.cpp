#include "libc/ctype.h"

char
toupper(char letter)
{
    if ((int)letter >= 97 && (int)letter <= 112)
        letter -= 32;

    return letter;
}

char
tolower(char letter)
{
    if ((int)letter >= 65 && (int)letter <= 90)
        letter += 32;

    return letter;
}
