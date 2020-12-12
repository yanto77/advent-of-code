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

    std::pair<uint8_t, uint8_t> get_row_col(uint16_t seat_id)
    {
        return { seat_id / 8, seat_id % 8 };
    }
}

void day05(const input_t& input)
{
    uint16_t max_seat_id = 0; // Part 1
    uint16_t my_seat_id = 0; // Part 2

    constexpr uint8_t MAX_ROW = 1 << 7;
    constexpr uint8_t MAX_COL = 1 << 3;
    std::array<std::array<bool, MAX_COL>, MAX_ROW> seats;
    for (auto& row: seats)
    {
        row.fill(false);
    }

    parse_input(input, [&](const std::string_view& line)
    {
        const auto& [row, col] = convert_line(line);
        uint16_t seat_id = get_seat_id(row, col);

        // Part 1
        if (seat_id > max_seat_id)
            max_seat_id = seat_id;

        // Part 2
        seats[row][col] = true;
    });


    // Part 2
    for (uint8_t row = 0; row < MAX_ROW; ++row)
    {
        for (uint8_t col = 0; col < MAX_COL; ++col)
        {
            if (seats[row][col] == false)
            {
                const uint16_t seat_id = get_seat_id(row, col);

                // Your seat is in the middle of the plane: the seats with
                // IDs +1 and -1 from yours will be [taken] in your list.
                const auto& [n_row, n_col] = get_row_col(seat_id + 1);
                const auto& [p_row, p_col] = get_row_col(seat_id - 1);
                const bool prev_seat_taken = seats[n_row][n_col];
                const bool next_seat_taken = seats[p_row][p_col];
                if (prev_seat_taken && next_seat_taken)
                {
                    my_seat_id = seat_id;
                    break;
                }
            }
        }

        if (my_seat_id != 0)
        {
            break;
        }
    }

    printf("max: %d, my: %d", max_seat_id, my_seat_id);
    assert(max_seat_id == 953);
    assert(my_seat_id == 615);
}

namespace // tests
{
    static_assert(convert_line("BFFFBBFRRR") == std::pair<uint8_t, uint8_t>{70, 7});
    static_assert(convert_line("FFFBBBFRRR") == std::pair<uint8_t, uint8_t>{14, 7});
    static_assert(convert_line("BBFFBBFRLL") == std::pair<uint8_t, uint8_t>{102, 4});
}
