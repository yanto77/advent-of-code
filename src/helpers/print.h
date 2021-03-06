#pragma once
#include <iostream>
#include <bitset>
#include <deque>
#include "vec2.h"


#define STR(x) #x
#define XSTR(x) STR(x)

#define USE_TRUE_COLOR 1
#ifdef USE_TRUE_COLOR
// https://gist.github.com/XVilka/8346728
#define TRUE_COLOR(R,G,B) "\x1b[38;2;" XSTR(R) ";" XSTR(G) ";" XSTR(B) "m"
#define COLOR_BLACK()   TRUE_COLOR(0, 0, 0)
#define COLOR_RED()     TRUE_COLOR(233, 47, 47)
#define COLOR_GREEN()   TRUE_COLOR(14, 216, 57)
#define COLOR_YELLOW()  TRUE_COLOR(221, 221, 19)
#define COLOR_BLUE()    TRUE_COLOR(59, 72, 227)
#define COLOR_MAGENTA() TRUE_COLOR(249, 150, 226)
#define COLOR_CYAN()    TRUE_COLOR(35, 237, 218)
#define COLOR_WHITE()   TRUE_COLOR(171, 171, 171)
#define COLOR_GRAY()    TRUE_COLOR(52, 52, 52)
#define COLOR_RESET()   "\033[39m"
#else
#define COLOR_BLACK()   "\033[30m"
#define COLOR_RED()     "\033[31m"
#define COLOR_GREEN()   "\033[32m"
#define COLOR_YELLOW()  "\033[33m"
#define COLOR_BLUE()    "\033[34m"
#define COLOR_MAGENTA() "\033[35m"
#define COLOR_CYAN()    "\033[36m"
#define COLOR_WHITE()   "\033[37m"
#define COLOR_GRAY()    "\033[38m"
#define COLOR_RESET()   "\033[39m"
#endif


template <typename T>
void print_col(const std::vector<T>& input)
{
    for (const auto& d : input)
    {
        std::cout << "  " << static_cast<int>(d) << '\n';
    }
}

template <typename T, size_t N>
void print_row(const std::array<T, N>& input)
{
    for (const auto& d : input)
    {
        printf("%d, ", static_cast<int>(d));
    }
    printf("\n");
}

template <size_t N>
inline void print_row(const std::array<std::string_view, N>& input)
{
    for (const auto& d : input)
    {
        std::cout << "[" << d << "], ";
    }
    printf("\n");
}

template <typename T>
void print_row(const std::vector<T>& input)
{
    for (const auto& d : input)
    {
        printf("%d, ", static_cast<int>(d));
    }
    printf("\n");
}

template <typename T>
void print_row(const std::deque<T>& input)
{
    for (const auto& d : input)
    {
        printf("%d, ", static_cast<int>(d));
    }
    printf("\n");
}

template <>
inline void print_row(const std::vector<std::string_view>& input)
{
    for (const auto& d : input)
    {
        std::cout << "[" << d << "], ";
    }
    printf("\n");
}

template <typename T>
void print_row(const std::vector<vec2<T>>& input)
{
    for (const auto& vec : input)
    {
        std::cout << "  (" << vec.x << ", " << vec.y << "), ";
    }
    std::cout << '\n';
}

inline void print_bits(uint8_t trg, bool color = false)
{
    for (int i = 7; i >= 0; --i)
    {
        if (color)
            printf(get_bit(trg, i) ? COLOR_GREEN() "1" : COLOR_CYAN() "0");
        else
            printf(get_bit(trg, i) ? "1" : "0");
        printf(COLOR_RESET());
    }
}

inline void print_bits(uint64_t trg, bool color = false)
{
    for (int i = 63; i >= 0; --i)
    {
        if (color)
            printf(get_bit(trg, i) ? COLOR_GREEN() "1" : COLOR_CYAN() "0");
        else
            printf(get_bit(trg, i) ? "1" : "0");
        printf(COLOR_RESET());
    }
}

template <size_t T>
inline void print_bits(std::bitset<T> trg, bool color = false)
{
    for (int i = T - 1; i >= 0; --i)
    {
        if (color)
            printf(trg.test(i) ? COLOR_GREEN() "1" : COLOR_CYAN() "0");
        else
            printf(trg.test(i) ? "1" : "0");
        printf(COLOR_RESET());
    }
}
