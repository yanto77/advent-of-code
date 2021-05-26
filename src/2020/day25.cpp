#include "setup/all.h"

ADVENT_DAY(2020, 25, 6408263, 1);

namespace
{
    constexpr size_t transform(size_t subject, size_t n_loops)
    {
        size_t value = 1;
        for (size_t idx = 0; idx < n_loops; ++idx)
        {
            value = (value * subject) % 20201227;
        }
        return value;
    }

    constexpr size_t get_loop_size(size_t input)
    {
        size_t loop_idx = 1;
        size_t value = 1;

        while (value != input)
        {
            value = (value * 7) % 20201227;
            ++loop_idx;
        }

        return loop_idx - 1;
    }
}

output_t Day_2020_25::run_solution(const input_t& /* input */) const
{
    size_t key1 = transform(8335663, get_loop_size(8614349));
    size_t key2 = transform(8614349, get_loop_size(8335663));
    assert(key1 == key2);
    return { key1, 1 };
}

void Day_2020_25::run_tests() const
{
    static_assert(8 == get_loop_size(5764801));
    static_assert(11 == get_loop_size(17807724));
    static_assert(14897079 == transform(17807724, get_loop_size(5764801)));
}
