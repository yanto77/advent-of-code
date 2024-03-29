#include "setup/all.h"

ADVENT_DAY(2020, 2, 465, 294);

namespace
{
    size_t count_occurences(str_view in, const char counted_ch)
    {
        return std::count_if(in.begin(), in.end(), [&](const char ch) { return ch == counted_ch; });
    }
}

result_t Day_2020_2::run_solution(str_view input) const
{
    size_t part1 = 0;
    size_t part2 = 0;

    for_each_split(input, '\n', [&](str_view line)
    {
        // Parsing line
        const auto& [lhs, pwd] = split_at(line, ": ");
        const auto& [range, chStr] = split_at(lhs, ' ');
        const auto& [minStr, maxStr] = split_at(range, '-');
        const char ch = chStr[0];
        const int min = to_int<int>(minStr);
        const int max = to_int<int>(maxStr);

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

void Day_2020_2::run_tests() const
{
    assert(count_occurences("abcdefg", 'c') == 1);
    assert(count_occurences("abababa", 'a') == 4);
    assert(count_occurences("abababa", 'b') == 3);
}
