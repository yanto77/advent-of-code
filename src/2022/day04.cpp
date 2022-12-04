#include "setup/all.h"

ADVENT_DAY(2022, 4, 562, 924);

namespace 
{
    std::array<uint8_t, 4> parse_line(str_view input)
    {
        // Data has numbers 1-99 (incl.).
        // Input:
        //      77-91,91-91
        //      3- 4, 3-17
        // Intermediate repr:
        //      [7, 7, 9, 1, 9, 1, 9, 1]
        //      [0, 3, 0, 4, 0, 3, 1, 7]
        // Output:
        //      [77, 91, 91, 91]
        //      [ 3,  4,  3, 17]

        std::array<uint8_t, 8> intermediate = {};
        size_t trg = (intermediate.size() - 1);
        for (int64_t src = (input.size() - 1); src >= 0; src--)
        {
            const char ch = input[src];
            if (ch == '-' || ch == ',')
            {
                // Next numbers start from odd indices (filled right to left). 
                if ((trg % 2) == 0) 
                {
                    trg--;
                }
                continue;
            }
            else 
            {
                intermediate[trg] = parse_uint(ch);
                trg--;
            }
        }

        std::array<uint8_t, 4> output;
        for (int i = 0; i < 4; i++)
        {
            output[i] = (intermediate[i*2] * 10) + intermediate[i*2+1];
        }

        return output;
    }
}

result_t Day_2022_4::run_solution(str_view input) const
{
    size_t part1 = 0;
    size_t part2 = 0;

    for_each_split(input, '\n', [&](str_view line)
    {
        auto [a1, b1, a2, b2] = parse_line(line);

        // a1--------b1
        //   a2---b2
        if (
            ((a2 <= a1 && a1 <= b2) && (a2 <= b1 && b1 <= b2)) ||
            ((a1 <= a2 && a2 <= b1) && (a1 <= b2 && b2 <= b1)))
        {
            part1 += 1;
        }

        // a1----b1
        //         a2---b2
        if (!(a2 > b1 || a1 > b2))
        {
            part2++;
        }
    });

    return { part1, part2 };
}

void Day_2022_4::run_tests() const
{
}
