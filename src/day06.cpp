#include "advent2020.h"
#include <iostream>
#include <numeric>

namespace
{
    template <typename T>
    constexpr void set_bit(T& trg, int bit)
    {
        trg |= 1 << (bit);
    }

    template <typename T>
    uint8_t count_bits(T n)
    {
        uint8_t count = 0;
        while (n) {
            count += n & 1;
            n >>= 1;
        }
        return count;
    }
}

void day06(const input_t& input)
{
    struct group_t
    {
        uint32_t p1 = 0; // anyone
        uint32_t p2 = UINT32_MAX; // everyone
    };

    std::vector<group_t> v { group_t() };
    v.reserve(500);

    size_t index = 0;

    parse_input(input, [&](const std::string_view& line)
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

    printf("part1: %hu, part2: %hu", acc1, acc2);
    assert(acc1 == 6947);
    assert(acc2 == 3398);
}
