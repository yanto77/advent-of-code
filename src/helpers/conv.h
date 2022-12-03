#pragma once
#include <charconv>
#include <cstdint>
#include <stdexcept>
#include <string_view>

#include "string.h"


/**
 * Convert single char into int
 * @param input input char
 * @returns parsed int
 */
template <typename T>
T to_int(char ch)
{
    return ch - '0';
}

/**
 * Convert full string into single int
 * @param input input string
 * @returns parsed int
 */
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

/**
 * Convert single hex char to integer representation
 * @param hex_digit hex char (e.g., '1', 'F')
 * @ref Source: https://stackoverflow.com/a/3382894
 * @returns int
 */
constexpr inline int hex_to_dec(unsigned char hex_digit)
{
    constexpr const signed char hex_values[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };

    int value = hex_values[hex_digit];
    if (value == -1)
        throw std::invalid_argument("invalid hex digit");

    return value;
}

/**
 * Convert full hex string into integer
 * @param input hex string, including # ("#123abc")
 * @returns int
 */
constexpr inline int hex_to_dec(str_view input)
{
    return hex_to_dec(input[1]) * 1048576 // 16^5
           + hex_to_dec(input[2]) * 65536 // 16^4
           + hex_to_dec(input[3]) * 4096 // 16^3
           + hex_to_dec(input[4]) * 256 // 16^2
           + hex_to_dec(input[5]) * 16 // 16^1
           + hex_to_dec(input[6]) * 1; // 16^0
}
