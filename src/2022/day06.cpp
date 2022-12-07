#include "setup/all.h"

ADVENT_DAY(2022, 6, 1625, 2250);

namespace 
{
    // returns index of first duplicate char, or -1 on valid string
    ssize_t valid_substr(str_view substr)
    {
        uint32_t chars = 0;
        for (int64_t i = (substr.size() - 1); i >= 0 ; i--)
        {
            size_t charIdx = (substr[i] - 'a');
            if (get_bit(chars, charIdx))
            {
                return i;
            }

            set_bit(chars, charIdx);
        }

        return -1;
    }

    size_t get_marker(str_view input, size_t msg_size)
    {
        for (size_t i = 0; i < (input.size() - msg_size); i++)
        {
            ssize_t res = valid_substr({ &input[i], msg_size});
            if (res == -1)
            {
                return i + msg_size;
            }
            else 
            {
                // with N being the index of _first_ duplicate char in the substr, 
                // we jump to N + 1, as that is the next possible substr without 
                // this duplicate char. (all the substrs in between will contain
                // the exact same duplicate char).
                i += res;
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
