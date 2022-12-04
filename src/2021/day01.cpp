#include "setup/all.h"

ADVENT_DAY(2021, 1, 1400, 1429);


result_t Day_2021_1::run_solution(str_view input) const
{
    std::vector<uint32_t> data;
    parse_uint_scalar(input, "\n", [&](uint32_t num)
    {
        data.push_back(num);
    });

    size_t part1 = 0;
    for (int i = 0, j = 1; j < data.size(); i++, j++)
    {
        uint32_t prev = data[i];
        uint32_t next = data[j];
        part1 += (next > prev);
    }

    size_t part2 = 0;
    for (int i = 2, j = 3; j < data.size(); i++, j++)
    {
        uint32_t prev = data[i - 2] + data[i - 1] + data[i];
        uint32_t next = data[j - 2] + data[j - 1] + data[j];
        part2 += (next > prev);
    }

    return { part1, part2 };
}

void Day_2021_1::run_tests() const
{
}
