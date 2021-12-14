#include "setup/all.h"

ADVENT_DAY(2021, 6, 383160, 1721148811504);

namespace
{
    size_t get_fish_count(const input_t& input, size_t days)
    {
        std::array<size_t, 9> buckets = {};
        for (int i = 0; i < input.len; i += 2)
        {
            buckets[to_int<size_t>(input.s[i])]++;
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

output_t Day_2021_6::run_solution(const input_t& input) const
{
    size_t part1 = get_fish_count(input, 80);
    size_t part2 = get_fish_count(input, 256);
    return { part1, part2 };
}

void Day_2021_6::run_tests() const
{
    char text1[] = "3,4,3,1,2\n";
    input_t test1 { text1, sizeof(text1) };

    assert(get_fish_count(test1, 18) == 26);
    assert(get_fish_count(test1, 80) == 5934);
    assert(get_fish_count(test1, 256) == 26984457539);
}
