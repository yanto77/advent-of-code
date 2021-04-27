#pragma once
#include <charconv>
#include <cstdint>
#include <stdexcept>
#include <string_view>

#include "string.h"

using sv = std::string_view;

/**
 * Convert full string into single int
 * @param input input string
 * @returns parsed int
 */
template <typename T>
T to_int(const sv& input)
{
    T n = 0;
    for (auto ch: input)
    {
        uint8_t d = ch - '0';
        if (d <= 9)
            n = 10 * n + d;
        else return n; // stop on first non-digit
    }

    return n;
}

/**
 * Convert first found int from `input` starting from `start`
 * @param input input string
 * @param start index to start looking from
 * @returns pair of { parsed int, index of next unparsed char }
 */
template <typename T>
std::pair<T, size_t> to_int(const sv& input, size_t start)
{
    // TODO: update to use to_int<T>() above??
    size_t end = input.find_first_not_of("0123456789", start);
    sv token { &input[start], end - start };
    return { to_int<T>(token), end };
}

/**
 * Convert string of multiple ints delimited by `delim`
 * @param input input string
 * @param input delimiter character
 * @returns vector of parsed ints
 */
template <typename T>
inline std::vector<T> to_multi_int(const sv& input, char delim)
{
    std::vector<T> out;
    for (const auto& part : split_multi(input, delim))
    {
        out.push_back(to_int<T>(part));
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
constexpr inline int hex_to_dec(const sv& input)
{
    return hex_to_dec(input[1]) * 1048576 // 16^5
           + hex_to_dec(input[2]) * 65536 // 16^4
           + hex_to_dec(input[3]) * 4096 // 16^3
           + hex_to_dec(input[4]) * 256 // 16^2
           + hex_to_dec(input[5]) * 16 // 16^1
           + hex_to_dec(input[6]) * 1; // 16^0
}
