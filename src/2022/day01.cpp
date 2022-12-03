#include "setup/all.h"

ADVENT_DAY(2022, 1, 69626, 206780);

result_t Day_2022_1::run_solution(str_view input) const
{
    std::array<size_t, 3> maxCounts = {};
    size_t currValue = 0;

    for_each_split(input, '\n', [&](str_view line)
    {
        if (line.empty())
        {
            if (maxCounts[0] == 0) maxCounts[0] = currValue;
            else if (maxCounts[1] == 0) maxCounts[1] = currValue;
            else if (maxCounts[2] == 0) maxCounts[2] = currValue;
            else if (currValue > maxCounts[0]) maxCounts[0] = currValue;
            else if (currValue > maxCounts[1]) maxCounts[1] = currValue;
            else if (currValue > maxCounts[2]) maxCounts[2] = currValue;
            std::sort(maxCounts.begin(), maxCounts.end());

            currValue = 0;
        }
        else 
        {
            currValue += to_int<size_t>(line);
        }
    });

    size_t part1 = maxCounts[2];
    size_t part2 = maxCounts[0] + maxCounts[1] + maxCounts[2];
    return { part1, part2 };
}

void Day_2022_1::run_tests() const
{
}
