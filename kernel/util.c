#include "util.h"

void memory_copy(const uint8_t* source, uint8_t* dest, uint32_t nbytes)
{
    // TODO: check for nullity
    int i = 0;
    for (i; i < nbytes; ++i) {
        dest[i] = source[i];
    }
}

void int_to_ascii(uint32_t n, uint8_t* result)
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

void memory_set(uint8_t* source, uint8_t value, uint32_t length)
{
    int i = 0;
    for (i; i < length; ++i) {
        source[i] = value;
    }
}