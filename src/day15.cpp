#include "advent2020.h"

namespace
{
    size_t solve(const std::vector<size_t>& input, size_t limit)
    {
        std::vector<size_t> history(limit, 0);
        for (size_t n = 0; n < input.size(); ++n)
        {
            history[input[n]] = n+1;
        }

        size_t value = 0;
        size_t last_diff = 0;
        for (size_t n = input.size(); n < limit; ++n)
        {
            size_t turn = n+1;

            value = last_diff;
            last_diff = (history[value] != 0) ? (turn - history[value]) : 0;
            history[value] = turn;
        }

        return value;
    }
}

output_t day15(const input_t& /* input */)
{
    size_t part1 = solve({1,0,16,5,17,4}, 2020);
    size_t part2 = solve({1,0,16,5,17,4}, 30000000);
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
    assert(solve({0,3,6}, 10) == 0);

    assert(solve({0,3,6}, 2020) == 436);
    assert(solve({1,3,2}, 2020) == 1);
    assert(solve({2,1,3}, 2020) == 10);
    assert(solve({1,2,3}, 2020) == 27);
    assert(solve({2,3,1}, 2020) == 78);
    assert(solve({3,2,1}, 2020) == 438);
    assert(solve({3,1,2}, 2020) == 1836);

    // NB: Takes approx 0.6 second per each call.
    // assert(solve({0,3,6}, 30000000) == 175594);
    // assert(solve({1,3,2}, 30000000) == 2578);
    // assert(solve({2,1,3}, 30000000) == 3544142);
    // assert(solve({1,2,3}, 30000000) == 261214);
    // assert(solve({2,3,1}, 30000000) == 6895259);
    // assert(solve({3,2,1}, 30000000) == 18);
    // assert(solve({3,1,2}, 30000000) == 362);
}
