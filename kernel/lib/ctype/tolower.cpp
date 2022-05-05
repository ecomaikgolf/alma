#include "lib/ctype.h"

char
tolower(char letter)
{
    if ((int)letter >= 65 && (int)letter <= 90)
        letter += 32;

    return letter;
}
