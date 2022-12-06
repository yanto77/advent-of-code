#include "setup/all.h"

ADVENT_DAY(2022, 6, 1625, 2250);

namespace 
{
    // returns index of first duplicate char, or -1 on valid string
    ssize_t valid_substr(str_view substr)
    {
        // NB: start at the end, as that will allow to find the _last_ duplicate 
        // index in _least_ number of operations, allowing large skips in future.
        for (int64_t i = (substr.size() - 1); i >= 0 ; i--)
        {
            for (int64_t j = i + 1; j < substr.size(); j++)
            {
                if (substr[i] == substr[j])
                {
                    return i;
                }
            }
        }

        return -1;
    }

    size_t get_marker(str_view input, size_t msg_size)
    {
        for (size_t lastIdx = (msg_size - 1); lastIdx < input.size(); lastIdx++)
        {
            size_t firstIdx = (lastIdx - (msg_size - 1));
            str_view substr { &input[firstIdx], msg_size};

            ssize_t res = valid_substr(substr);
            if (res == -1)
            {
                return lastIdx + 1; // convert 0-index to 1-index
            }
            else 
            {
                // with N being the index of _first_ duplicate char in the substr, 
                // we jump to N + 1, as that is the next possible substr without 
                // this duplicate char. (all the substrs in between will contain
                // the exact same duplicate char).
                lastIdx = lastIdx + res;
            }
        }

        assert(false); // we expect to find a valid marker in every line
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
