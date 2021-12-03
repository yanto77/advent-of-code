#include "setup/all.h"

ADVENT_DAY(2021, 1, 1400, 1429);


output_t Day_2021_1::run_solution(const input_t& input) const
{
    std::vector<int32_t> data;
    parse_input(input, [&](const sv& line)
    {
        data.push_back(to_int<int32_t>(line));
    });

    size_t part1 = 0;
    for (int i = 0, j = 1; j < data.size(); i++, j++)
    {
        int32_t prev = data[i];
        int32_t next = data[j];
        part1 += (next > prev);
    }

    size_t part2 = 0;
    for (int i = 2, j = 3; j < data.size(); i++, j++)
    {
        int32_t prev = data[i - 2] + data[i - 1] + data[i];
        int32_t next = data[j - 2] + data[j - 1] + data[j];
        part2 += (next > prev);
    }

    return { part1, part2 };
}

void Day_2021_1::run_tests() const
{
}
