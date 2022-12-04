#pragma once
#include <charconv>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <cassert>
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

inline uint8_t parse_uint(char ch) 
{
    return ch - '0'; 
}

template <typename Functor>
void parse_uint_scalar(str_view input, str_view separators, Functor func)
{
    uint64_t number = 0;
    uint8_t n_digits = 0;

    for (int i = 0; i < input.size(); i ++)
    {
        const char ch = input[i];
        if ('0' <= ch && ch <= '9')
        {
            number = 10 * number + parse_uint(ch);
            n_digits++;
        }
        else if (separators.find_first_of(ch) != str_view::npos)
        {
            if (n_digits > 0)
            {
                func(number);
                number = 0;
                n_digits = 0;
            }
        }
        else assert(false);
    }

    if (n_digits > 0)
    {
        func(number);
    }
}

template <typename Functor>
void parse_int_scalar(str_view input, str_view separators, Functor func)
{
    // aoc doesn't use `+`, so assume the numbers are positive unless there's a minus.
    int8_t sign = +1; 
    uint64_t number = 0;
    uint8_t n_digits = 0;

    for (int i = 0; i < input.size(); i ++)
    {
        const char ch = input[i];
        if ('0' <= ch && ch <= '9')
        {
            number = 10 * number + parse_uint(ch);
            n_digits++;
        }
        else if (ch == '-')
        {
            sign = -1;
        }
        else if (separators.find_first_of(ch) != str_view::npos)
        {
            if (n_digits > 0)
            {
                func(sign * number);
                sign = +1;
                number = 0;
                n_digits = 0;
            }
        }
        else assert(false);
    }

    if (n_digits > 0)
    {
        func(sign * number);
    }
}
