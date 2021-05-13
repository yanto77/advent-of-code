#include "advent2020.h"

namespace
{
    uint32_t solve(const std::vector<uint32_t>& input, uint32_t limit)
    {
        std::vector<uint32_t> history(limit, 0);

        const uint32_t N = static_cast<uint32_t>(input.size());
        for (uint32_t n = 0; n < N; ++n)
        {
            history[input[n]] = n + 1;
        }

        uint32_t spoken = 0;
        for (uint32_t turn = N + 1; turn < limit; ++turn)
        {
            uint32_t hist_val = std::exchange(history[spoken], turn);
            spoken = (hist_val != 0) ? (turn - hist_val) : 0;
        }

        return spoken;
    }
}

output_t day15(const input_t& /* input */)
{
    size_t part1 = solve({ 1, 0, 16, 5, 17, 4 }, 2020);
    size_t part2 = solve({ 1, 0, 16, 5, 17, 4 }, 30000000);
    return { part1, part2 };
}

void day15_test()
{
    // Turn  1:                              Value=0, Insert={1, 0}
    // Turn  2:                              Value=3, Insert={2, 0}
    // Turn  3:                              Value=6, Insert={3, 0}
    // Turn  4: LastValue=6. History={3, 0}, Value=0, Update={4, 1}
    // Turn  5: LastValue=0. History={4, 1}, Value=3, Update={5, 2}
    // Turn  6: LastValue=3, History={5, 2}, Value=3, Update={6, 5}
    // Turn  7: LastValue=3, History={6, 5}, Value=1, Insert={7, 0}
    // Turn  8: LastValue=1, History={7, 0}, Value=0, Update={8, 4}
    // Turn  9: LastValue=0, History={8, 4}, Value=4, Update={9, 0}
    // Turn 10: LastValue=4, History={9, 0}, Value=0, Update={10, 8}
    assert(solve({ 0, 3, 6 }, 10) == 0);

    assert(solve({ 0, 3, 6 }, 2020) == 436);
    assert(solve({ 1, 3, 2 }, 2020) == 1);
    assert(solve({ 2, 1, 3 }, 2020) == 10);
    assert(solve({ 1, 2, 3 }, 2020) == 27);
    assert(solve({ 2, 3, 1 }, 2020) == 78);
    assert(solve({ 3, 2, 1 }, 2020) == 438);
    assert(solve({ 3, 1, 2 }, 2020) == 1836);

    // NB: Takes approx 0.6 second per each call.
    // assert(solve({0,3,6}, 30000000) == 175594);
    // assert(solve({1,3,2}, 30000000) == 2578);
    // assert(solve({2,1,3}, 30000000) == 3544142);
    // assert(solve({1,2,3}, 30000000) == 261214);
    // assert(solve({2,3,1}, 30000000) == 6895259);
    // assert(solve({3,2,1}, 30000000) == 18);
    // assert(solve({3,1,2}, 30000000) == 362);
}
