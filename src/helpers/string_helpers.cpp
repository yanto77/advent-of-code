#include "string_helpers.h"

// Original "splitSVPtr", source: https://www.bfilipek.com/2018/07/string-view-perf-followup.html
std::vector<sv> split(sv str, sv delims)
{
    std::vector<sv> output;

    for (auto first = str.data(), second = str.data(), last = first + str.size();
        second != last && first != last;
        first = second + 1)
    {
        second = std::find_first_of(first, last, std::cbegin(delims), std::cend(delims));

        if (first != second)
        {
            output.emplace_back(first, second - first);
        }
    }

    return output;
}

std::pair<sv, sv> split_single(const sv& input, const sv& delim)
{
    size_t lhs_end = input.find(delim);
    size_t rhs_start = lhs_end + delim.size();

    sv lhs { &input[0], lhs_end };
    sv rhs { &input[rhs_start], (input.size() - rhs_start) };

    return { lhs, rhs };
}

std::vector<sv> split_multi(const sv& input, char delim)
{
    std::vector<sv> out;

    // value: 3 shiny tomato bags, 2 light fuchsia bags, 1 bright salmon bag.
    //        012345679012345679012345679012345679012345679012345679012345679
    //                          21                     45
    //            {0, 21 -1 = 20}         (22, 45-22 = 23 -1 = 22)

    size_t start = 0;
    while (start < input.size())
    {
        size_t pos = input.find_first_of(delim, start);
        if (pos == sv::npos)
            break;

        size_t substr_len = pos - start;
        out.push_back(sv(&input[start], substr_len));
        start = pos + 2; // skip delim + whitespace after
    }

    size_t last_size = input.size() - start - 1; // remove last period
    out.push_back(sv(&input[start], last_size));

    return out;
}
