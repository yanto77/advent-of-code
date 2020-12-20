#include "bit_helpers.h"
#include <cstdio>

void print_bits(uint8_t trg)
{
    for (int i = 0; i < 8; ++i)
    {
        printf(get_bit(trg, i) ? "1" : "0");
    }
}
