#include "setup/all.h"

ADVENT_DAY(2021, 6, 383160, 1721148811504);

namespace
{
    size_t get_fish_count(str_view input, size_t days)
    {
        std::array<size_t, 9> buckets = {};
        for (int i = 0; i < input.size(); i += 2)
        {
            uint8_t idx = parse_uint(input[i]);
            buckets[idx]++;
        }

        for (int d = 0; d < days; d++)
        {
            // shift
            auto temp = buckets[0];
            buckets[0] = buckets[1];
            buckets[1] = buckets[2];
            buckets[2] = buckets[3];
            buckets[3] = buckets[4];
            buckets[4] = buckets[5];
            buckets[5] = buckets[6];
            buckets[6] = buckets[7];
            buckets[7] = buckets[8];
            buckets[8] = temp;

            // add new
            buckets[6] += temp;
        }

        size_t sum = 0;
        for (auto bucket: buckets)
        {
            sum += bucket;
        }

        return sum;
    }
}

result_t Day_2021_6::run_solution(str_view input) const
{
    size_t part1 = get_fish_count(input, 80);
    size_t part2 = get_fish_count(input, 256);
    return { part1, part2 };
}

void Day_2021_6::run_tests() const
{
    char text1[] = "3,4,3,1,2\n";

    assert(get_fish_count(text1, 18) == 26);
    assert(get_fish_count(text1, 80) == 5934);
    assert(get_fish_count(text1, 256) == 26984457539);
}
