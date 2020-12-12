#include "advent2020.h"

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

    uint16_t get_seat_id(uint8_t row, uint8_t col)
    {
        return row * 8 + col;
    }
}

void day05(const input_t& input)
{
    uint16_t max_seat_id = 0; // Part 1
    parse_input(input, [&](const std::string_view& line)
    {
        const auto& [row, col] = convert_line(line);
        uint16_t seat_id = get_seat_id(row, col);
        if (seat_id > max_seat_id)
            max_seat_id = seat_id;
    });

    printf("%d", max_seat_id);
    assert(max_seat_id == 953);
}

namespace // tests
{
    static_assert(convert_line("BFFFBBFRRR") == std::pair<uint8_t, uint8_t>{70, 7});
    static_assert(convert_line("FFFBBBFRRR") == std::pair<uint8_t, uint8_t>{14, 7});
    static_assert(convert_line("BBFFBBFRLL") == std::pair<uint8_t, uint8_t>{102, 4});
}
