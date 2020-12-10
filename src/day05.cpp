#include "advent2020.h"
#include <iostream>

namespace
{
    #define SET_BIT(trg, idx) trg |= 1 << (idx)

    constexpr std::pair<uint8_t, uint8_t> convert_line(const std::string_view& sv)
    {
        // withing this method, we don't explicitly set 0 bits,
        // as those are already 0 when initialized

        uint8_t row = 0;
        for (int i = 0; i < 7; ++i)
            if (sv[i] == 'B')
                SET_BIT(row, (7-1-i));

        uint8_t col = 0;
        for (int i = 0; i < 3; ++i)
            if (sv[7+i] == 'R')
                SET_BIT(col, (3-1-i));

        return {row, col};
    }
}

void day05(input_t input)
{
    printf("Day 05: todo\n");

    static_assert(convert_line("BFFFBBFRRR") == std::pair<uint8_t, uint8_t>{70, 7});
    static_assert(convert_line("FFFBBBFRRR") == std::pair<uint8_t, uint8_t>{14, 7});
    static_assert(convert_line("BBFFBBFRLL") == std::pair<uint8_t, uint8_t>{102, 4});
}
