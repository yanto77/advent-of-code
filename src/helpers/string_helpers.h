#pragma once
#include <string_view>
#include <vector>

using sv = std::string_view;

// Original "splitSVPtr", source: https://www.bfilipek.com/2018/07/string-view-perf-followup.html
inline std::vector<sv> split(sv str, sv delims)
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

inline std::pair<sv, sv> split_single(const sv& input, const sv& delim)
{
    size_t lhs_end = input.find(delim);
    size_t rhs_start = lhs_end + delim.size();

    sv lhs { &input[0], lhs_end };
    sv rhs { &input[rhs_start], (input.size() - rhs_start) };

    return { lhs, rhs };
}

inline std::vector<sv> split_multi(const sv& input, const sv& delim)
{
    std::vector<sv> out;

    size_t start = 0;
    while (start < input.size())
    {
        size_t pos = input.find(delim, start);
        if (pos == sv::npos)
        {
            if (start < input.size()) // fill in the last part
                out.push_back(sv{ &input[start], input.size() - start });

            break;
        }

        out.push_back(sv { &input[start], pos - start });
        start = pos + delim.size();
    }

    return out;
}

inline std::vector<sv> split_multi(const sv& input, char delim)
{
    return split_multi(input, sv { &delim, 1 });
}
