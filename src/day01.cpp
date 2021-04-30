#include "advent2020.h"

namespace
{
    size_t part1_sum(const std::vector<int>& v)
    {
        for (int i : v)
            for (int j : v)
                if (i + j == 2020)
                    return i * j;

        return 0;
    }

    size_t part2_sum(const std::vector<int>& v)
    {
        const int min = v[0];
        for (int i : v)
        {
            for (int j : v)
            {
                if (i + j + min >= 2020)
                    break;

                for (int k : v)
                {
                    if (i + j + k == 2020)
                        return i * j * k;
                }
            }
        }

        return 0;
    }
}

output_t day01(const input_t& input)
{
    std::vector<int> v;
    v.reserve(1000);
    parse_input(input, [&](const sv& line)
    {
        v.emplace_back(to_int<int>(line));
    });

    std::sort(v.begin(), v.end());

    return { part1_sum(v), part2_sum(v) };
}

void day01_test()
{
}
