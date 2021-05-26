#include "setup/all.h"

ADVENT_DAY(2019, 1, 3520097, 5277255);

namespace
{
    constexpr int32_t fn1(int32_t in)
    {
        return (in / 3) - 2;
    }

    constexpr int32_t fn2(int32_t in)
    {
        auto out = (in / 3) - 2;
        if (out > 0)
            return out + fn2(out);
        else
            return 0;
    }
}

void Day_2019_1::run_tests() const
{
    static_assert(fn1(12) == 2);
    static_assert(fn1(14) == 2);
    static_assert(fn1(1969) == 654);
    static_assert(fn1(100756) == 33583);

    static_assert(fn2(12) == 2);
    static_assert(fn2(14) == 2);
    static_assert(fn2(1969) == 966);
    static_assert(fn2(100756) == 50346);
}

output_t Day_2019_1::run_solution(const input_t& input) const
{
    size_t sum1 = 0;
    size_t sum2 = 0;
    parse_input(input, [&](const sv& line)
    {
        auto value = to_int<int32_t>(line);
        sum1 += fn1(value);
        sum2 += fn2(value);
    });

    return {sum1, sum2};
}
