#include "advent2020.h"

output_t day01(const input_t& input)
{
    std::vector<int> v;
    v.reserve(1000);
    parse_input(input, [&](const sv& line)
    {
        v.emplace_back(to_int<int>(line));
    });

    std::sort(v.begin(), v.end());

    // Part 1
    size_t part1 = 0;
    for (int i : v)
    {
        for (int j : v)
        {
            if (i + j == 2020)
            {
                part1 = i * j;
                break;
            }
        }

        if (part1 != 0)
            break;
    }

    // Part 2
    size_t part2 = 0;
    for (int i : v)
    {
        for (int j : v)
        {
            for (int k : v)
            {
                if (i + j + k == 2020)
                {
                    part2 = i * j * k;
                    break;
                }
            }

            if (part2 != 0)
                break;
        }

        if (part2 != 0)
            break;
    }

    return { part1, part2 };
}

void day01_test()
{
}
