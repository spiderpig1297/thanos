#include "util.h"

void memory_copy(const char* source, char* dest, int nbytes)
{
    // TODO: check for nullity
    int i = 0;
    for (i; i < nbytes; ++i) {
        dest[i] = source[i];
    }
}

void int_to_ascii(int n, char *result)
{
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        result[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) result[i++] = '-';
    result[i] = '\0';
}