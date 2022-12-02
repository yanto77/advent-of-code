#pragma once
#include <string_view>
#include <vector>
#include <functional>

using str_view = std::string_view;

/**
 * Split string by delimiters into multiple parts
 */
inline std::vector<str_view> split_multi(str_view input, str_view delim)
{
    std::vector<str_view> out;

    size_t start = 0;
    while (start < input.size())
    {
        size_t pos = input.find(delim, start);
        if (pos == str_view::npos)
        {
            if (start < input.size()) // fill in the last part
                out.push_back(str_view { &input[start], input.size() - start });

            break;
        }

        out.push_back(str_view { &input[start], pos - start });
        start = pos + delim.size();
    }

    return out;
}

/**
 * Split string by delimiters into multiple parts, run `cb` on each part
 */
inline void split_multi(str_view input, str_view delim, std::function<void(str_view)> callback)
{
    size_t start = 0;
    while (start < input.size())
    {
        size_t pos = input.find(delim, start);
        if (pos == str_view::npos)
        {
            if (start < input.size()) // fill in the last part
                callback(str_view { &input[start], input.size() - start });

            break;
        }

        callback(str_view { &input[start], pos - start });
        start = pos + delim.size();
    }
}

/**
 * Split string by delimiters into multiple parts
 */
inline std::vector<str_view> split_multi(str_view input, char delim)
{
    return split_multi(input, str_view { &delim, 1 });
}

inline void split_multi(str_view input, char delim, std::function<void(str_view)> callback)
{
    split_multi(input, str_view { &delim, 1 }, callback);
}

/**
 * Split string by delimiters into two parts
 * @returns pair of the two parts
 */
inline std::pair<str_view, str_view> split_single(str_view input, str_view delim)
{
    size_t lhs_end = input.find(delim);
    size_t rhs_start = lhs_end + delim.size();

    str_view lhs { &input[0], lhs_end };
    str_view rhs { &input[rhs_start], (input.size() - rhs_start) };

    return { lhs, rhs };
}
