#include "advent2020.h"

ADVENT_DAY(2020, 5, 953, 615);

namespace
{
    constexpr uint16_t convert_line(const sv& sv)
    {
        // withing this method, we don't explicitly set 0 bits,
        // as those are already 0 when initialized
        uint16_t result = 0;

        for (uint8_t i = 0; i < 7; ++i)
            if (sv[i] == 'B')
                set_bit(result, (9 - i));

        for (uint8_t i = 0; i < 3; ++i)
            if (sv[7 + i] == 'R')
                set_bit(result, (2 - i));

        return result;
    }
}

output_t Day_2020_5::run_solution(const input_t& input) const
{
    uint16_t max_seat_id = 0; // Part 1
    uint16_t my_seat_id = 0; // Part 2

    constexpr uint16_t SEAT_NB = (1 << 10);
    std::array<bool, SEAT_NB> seats;
    seats.fill(false);

    parse_input(input, [&](const sv& line)
    {
        uint16_t seat_id = convert_line(line);

        // Part 1
        if (seat_id > max_seat_id)
            max_seat_id = seat_id;

        // Part 2
        seats[seat_id] = true;
    });

    // Part 2
    for (uint16_t i = 0; i < SEAT_NB - 2; ++i)
    {
        if (seats[i] && !seats[i + 1] && seats[i + 2])
        {
            my_seat_id = i + 1;
            break;
        }
    }

    return { max_seat_id, my_seat_id };
}

void Day_2020_5::run_tests() const
{
    static_assert(convert_line("BFFFBBFRRR") == 70 * 8 + 7);
    static_assert(convert_line("FFFBBBFRRR") == 14 * 8 + 7);
    static_assert(convert_line("BBFFBBFRLL") == 102 * 8 + 4);
}
