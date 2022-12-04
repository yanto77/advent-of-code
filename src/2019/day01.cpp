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

result_t Day_2019_1::run_solution(str_view input) const
{
    size_t sum1 = 0;
    size_t sum2 = 0;
    parse_uint_scalar(input, "\n", [&](uint32_t num)
    {
        sum1 += fn1(num);
        sum2 += fn2(num);
    });

    return {sum1, sum2};
}
