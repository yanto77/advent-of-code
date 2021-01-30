#pragma once
#include <string_view>
#include <vector>

using sv = std::string_view;

template <typename T>
inline std::vector<T> parse_ints(const sv& input, char delim)
{
    std::vector<T> out;

    size_t start = 0;
    while (start < input.size())
    {
        size_t pos = input.find_first_of(delim, start);
        if (pos == sv::npos)
            break;

        size_t substr_len = pos - start;
        out.push_back(to_int<T>(sv{ &input[start], substr_len }));

        start = pos + 1; // skip delim
    }

    size_t last_size = input.size() - start; // remove last period
    out.push_back(to_int<T>(sv{ &input[start], last_size }));

    return out;
}

template <typename T>
inline std::pair<T, size_t> parse_num(const sv& input, size_t start)
{
    size_t end = input.find_first_not_of("0123456789", start);
    sv token { &input[start], end - start };
    return { to_int<T>(token), end };
}
