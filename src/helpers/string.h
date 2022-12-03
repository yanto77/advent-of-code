#pragma once
#include <string_view>
#include <vector>

using str_view = std::string_view;

// Split string by delimiters into multiple parts, run `cb` on each part
template <typename Functor>
inline void for_each_split(str_view input, str_view delim, Functor&& callback)
{
    size_t start = 0U;
    size_t end = input.find(delim);
    while (end != std::string::npos)
    {
        callback(str_view { &input[start], (end - start) });
        start = end + delim.length();
        end = input.find(delim, start);
    }

    if (start < input.size())
    {
        callback(str_view { &input[start], (input.size() - start) });
    }
}

// Split string by delimiters into multiple parts, run `cb` on each part
template <typename Functor>
inline void for_each_split(str_view input, char delim, Functor&& callback)
{
    size_t start = 0U;
    size_t end = input.find(delim);
    while (end != std::string::npos)
    {
        callback(str_view { &input[start], (end - start) });
        start = end + 1;
        end = input.find(delim, start);
    }

    if (start < input.size())
    {
        callback(str_view { &input[start], (input.size() - start) });
    }
}

// Split string by delimiters into multiple parts
inline std::vector<str_view> split(str_view input, str_view delim)
{
    std::vector<str_view> out;
    for_each_split(input, delim, [&](str_view part)
    {
        out.push_back(part);
    });
    return out;
}

// Split string by delimiters into multiple parts
inline std::vector<str_view> split(str_view input, char delim)
{
    std::vector<str_view> out;
    for_each_split(input, delim, [&](str_view part)
    {
        out.push_back(part);
    });
    return out;
}

// Split string at index into two parts
inline std::pair<str_view, str_view> split_at(str_view input, size_t idx, size_t pad = 0)
{
    str_view lhs { &input[0], idx };
    str_view rhs { &input[(idx + pad)], (input.size() - (idx + pad)) };
    return { lhs, rhs };
}

// Split string by delimiter into two parts
inline std::pair<str_view, str_view> split_at(str_view input, str_view delim)
{
    return split_at(input, input.find(delim), delim.size());
}

// Split string by delimiter into two parts
inline std::pair<str_view, str_view> split_at(str_view input, char delim)
{
    return split_at(input, input.find(delim), 1);
}

