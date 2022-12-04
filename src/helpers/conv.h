#pragma once
#include <charconv>
#include <cstdint>
#include <stdexcept>
#include <string_view>

#include "string.h"


// Convert a string into 1 int
template <typename T>
T to_int(str_view input)
{
    int8_t sgn = +1;
    T num = 0;

    size_t ch_idx = input.find_first_of("-0123456789");
    while (ch_idx < input.size())
    {
        char ch = input[ch_idx++];
        if (ch == '-')
            sgn = -1;
        else if (uint8_t d = ch - '0'; d <= 9)
            num = 10 * num + d;
        else
            return sgn * num; // stop on first non-digit
    }

    return sgn * num;
}

/**
 * Convert first found int from `input` starting from `start`
 * @param input input string
 * @param start index to start looking from
 * @returns pair of { parsed int, index of next unparsed char }
 */
template <typename T>
std::pair<T, size_t> to_int(str_view input, size_t start)
{
    size_t end = input.find_first_not_of("-0123456789", start);
    str_view token { &input[start], end - start };
    return { to_int<T>(token), end };
}

/**
 * Convert string of multiple ints delimited by single non-numeric char
 * @param input input string
 * @returns vector of parsed ints
 */
template <typename T>
inline std::vector<T> to_multi_int(str_view input)
{
    std::vector<T> out;

    size_t ch_idx = 0; // line character index
    while (ch_idx < input.size())
    {
        const auto& [num, end] = to_int<T>(input, ch_idx);
        out.push_back(num);
        ch_idx = end + 1;

        if (end == SIZE_MAX)
            break;
    }

    return out;
}

inline uint8_t parse_uint(char ch) 
{
    return ch - '0'; 
}
