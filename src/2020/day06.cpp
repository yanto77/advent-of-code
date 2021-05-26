#include "setup/all.h"

ADVENT_DAY(2020, 6, 6947, 3398);

output_t Day_2020_6::run_solution(const input_t& input) const
{
    struct group_t
    {
        uint32_t p1 = 0; // anyone
        uint32_t p2 = UINT32_MAX; // everyone
    };

    std::vector<group_t> v { group_t() };
    v.reserve(500);

    size_t index = 0;

    parse_input(input, [&](const sv& line)
    {
        if (line.empty())
        {
            v.push_back(group_t());
            ++index;
        }
        else
        {
            uint32_t input_bits = 0;
            for (int i = 0; i < line.size(); ++i)
            {
                set_bit(input_bits, line[i] - static_cast<int>('a'));
            }

            v[index].p1 |= input_bits;
            v[index].p2 &= input_bits;
        }
    });

    uint16_t acc1 = 0;
    uint16_t acc2 = 0;
    for (size_t i = 0; i <= index; ++i)
    {
        acc1 += count_bits(v[i].p1);
        acc2 += count_bits(v[i].p2);
    }

    return { acc1, acc2 };
}

void Day_2020_6::run_tests() const
{
}
