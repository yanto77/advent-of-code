#include "advent2020.h"

namespace
{
    size_t count_occurences(const sv& in, const char counted_ch)
    {
        return std::count_if(in.begin(), in.end(), [&](const char ch) { return ch == counted_ch; });
    }
}

output_t day02(const input_t& input)
{
    size_t part1 = 0;
    size_t part2 = 0;

    parse_input(input, [&](const sv& line)
    {
        // Parsing line
        const auto& [min, idx1] = to_int<int>(line, 0);
        const auto& [max, idx2] = to_int<int>(line, idx1+1);
        const char ch = line[idx2+1];
        const sv pwd { &line[idx2+4], line.size() - idx2 - 4};

        // Part 1
        size_t count = count_occurences(pwd, ch);
        part1 += (min <= count && count <= max);

        // Part 2
        bool pos1 = (pwd[min - 1] == ch);
        bool pos2 = (pwd[max - 1] == ch);
        part2 += (pos1 ^ pos2);
    });

    return { part1, part2 };
}

void day02_test()
{
    assert(count_occurences("abcdefg", 'c') == 1);
    assert(count_occurences("abababa", 'a') == 4);
    assert(count_occurences("abababa", 'b') == 3);
}
