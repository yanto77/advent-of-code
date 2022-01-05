#pragma once
#include "vec2.h"
#include <bitset>
#include <iostream>

#define STR(x)  #x
#define XSTR(x) STR(x)

#define USE_TRUE_COLOR 1
#ifdef USE_TRUE_COLOR
    // https://gist.github.com/XVilka/8346728
    #define TRUE_COLOR(R, G, B) "\x1b[38;2;" XSTR(R) ";" XSTR(G) ";" XSTR(B) "m"
    #define COLOR_BLACK()       TRUE_COLOR(0, 0, 0)
    #define COLOR_RED()         TRUE_COLOR(233, 47, 47)
    #define COLOR_GREEN()       TRUE_COLOR(14, 216, 57)
    #define COLOR_YELLOW()      TRUE_COLOR(221, 221, 19)
    #define COLOR_BLUE()        TRUE_COLOR(59, 72, 227)
    #define COLOR_MAGENTA()     TRUE_COLOR(249, 150, 226)
    #define COLOR_CYAN()        TRUE_COLOR(35, 237, 218)
    #define COLOR_WHITE()       TRUE_COLOR(171, 171, 171)
    #define COLOR_GRAY()        TRUE_COLOR(52, 52, 52)
    #define COLOR_RESET()       "\033[39m"
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

// TODO: to_string() methods?

inline void clear_screen()
{
    // clear screen and move cursor to top-left corner
    printf("\x1B[2J\x1B[H");
}

template <typename cnt>
void print_col(const cnt& data)
{
    for (const auto& d : data)
    {
        printf("%d\n", static_cast<int>(d));
    }
}

template <typename cnt>
void print_row(const cnt& data, ssize_t linebreak = -1)
{
    for (size_t idx = 0; idx < data.size(); ++idx)
    {
        printf("%d, ", static_cast<int>(data[idx]));

        if (linebreak != -1 && (idx + 1) % linebreak == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

template <typename cnt>
void print_row_text(const cnt& data)
{
    for (const auto& d : data)
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

template <typename T, size_t N = sizeof(T) * 8>
void print_bits(T data, bool color = false)
{
    for (int i = N - 1; i >= 0; --i)
    {
        if (color)
            printf(get_bit(data, i) ? COLOR_GREEN() "1" : COLOR_CYAN() "0");
        else
            printf(get_bit(data, i) ? "1" : "0");
        printf(COLOR_RESET());
    }
}

template <size_t N>
void print_bits(const std::bitset<N>& data, bool color = false)
{
    for (int i = N - 1; i >= 0; --i)
    {
        if (color)
            printf(data.test(i) ? COLOR_GREEN() "1" : COLOR_CYAN() "0");
        else
            printf(data.test(i) ? "1" : "0");
        printf(COLOR_RESET());
    }
}

inline void print_bits(const __m256i& data, bool color = false)
{
    const size_t n = sizeof(__m256i) / sizeof(uint8_t);
    uint8_t buffer[n];

    _mm256_storeu_si256((__m256i*)buffer, data);

    for (int i = 0; i < n; i++)
    {
        print_bits(buffer[i], color); printf(" ");
    }
}
