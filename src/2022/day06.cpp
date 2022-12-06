#include "setup/all.h"

ADVENT_DAY(2022, 6, 1625, 2250);

namespace 
{
    size_t get_marker(str_view input, size_t msg_size)
    {
        for (size_t lastIdx = (msg_size - 1); lastIdx < input.size(); lastIdx++)
        {
            bool no_duplicates = true;
            for (size_t i = (lastIdx - msg_size + 1); i < lastIdx && no_duplicates; i++)
            {
                for (size_t j = i + 1; j <= lastIdx && no_duplicates; j++)
                {
                    no_duplicates &= (input[i] != input[j]);
                }
            }

            if (no_duplicates)
            {
                return lastIdx + 1;
            }
        }

        assert(false);
        return 0;
    }
}

result_t Day_2022_6::run_solution(str_view input) const
{
    size_t part1 = get_marker(input, 4);
    size_t part2 = get_marker(input, 14);
    return { part1, part2 };
}

void Day_2022_6::run_tests() const
{
    struct test_t { const char* input; int expected_packet; int expected_message; };

    test_t values[] = {
        { "mjqjpqmgbljsphdztnvjfqwrcgsmlb", 7, 19 },
        { "bvwbjplbgvbhsrlpgdmjqwftvncz", 5, 23 },
        { "nppdvjthqldpwncqszvftbrmjlhg", 6, 23 },
        { "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 10, 29 },
        { "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 11, 26 },
    };

    for (auto [input, exp_packet, exp_message]: values)
    { 
        size_t packet = get_marker(input, 4);
        size_t message = get_marker(input, 14);
        // fmt::print("input: {}, packet {} (exp: {}), message: {} (exp:{})\n", input, packet, exp_packet, message, exp_message);
        assert(packet == exp_packet);
        assert(message == exp_message);
    }
}
