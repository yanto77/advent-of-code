#include "setup/all.h"

ADVENT_DAY(2020, 6, 6947, 3398);

result_t Day_2020_6::run_solution(str_view input) const
{
    struct group_t
    {
        std::bitset<32> p1 { 0 }; // anyone
        std::bitset<32> p2 { UINT32_MAX }; // everyone
    };

    std::vector<group_t> v { group_t() };
    v.reserve(500);

    size_t index = 0;

    parse_input(input, [&](str_view line)
    {
        if (line.empty())
        {
            v.push_back(group_t());
            ++index;
        }
        else
        {
            std::bitset<32> input_bits = 0;
            for (int i = 0; i < line.size(); ++i)
            {
                input_bits.set(line[i] - static_cast<int>('a'));
            }

            v[index].p1 |= input_bits;
            v[index].p2 &= input_bits;
        }
    });

    uint16_t acc1 = 0;
    uint16_t acc2 = 0;
    for (size_t i = 0; i <= index; ++i)
    {
        acc1 += v[i].p1.count();
        acc2 += v[i].p2.count();
    }

    return { acc1, acc2 };
}

void Day_2020_6::run_tests() const
{
}
