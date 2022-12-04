#pragma once
#include "vec2.h"
#include <bitset>
#include <iostream>
#include <immintrin.h>

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

inline void clear_screen()
{
    // clear screen and move cursor to top-left corner
    printf("\x1B[2J\x1B[H");
}

void print_bits(uint8_t* data, size_t count, bool color);

void print_bits_ext(uint8_t* data, size_t count, bool color);

// Prints values as bits
template <typename T>
void print_bits(T data, bool color = false)
{
    constexpr size_t n = sizeof(T) / sizeof(uint8_t);
    uint8_t buffer[n];

    if constexpr (std::is_same<T, __m128i>::value) 
    {
        _mm_storeu_si128((__m128i*)buffer, data);
    }
    else if constexpr (std::is_same<T, __m256i>::value)
    {
        _mm256_storeu_si256((__m256i*)buffer, data);
    }
    else 
    {
        std::memcpy(buffer, &data, sizeof(data));
    }

    print_bits(buffer, n, color);
}

// Prints values as bits, hex, and chars
template <typename T>
void print_bits_ext(T data, bool color = false)
{
    constexpr size_t n = sizeof(T) / sizeof(uint8_t);
    uint8_t buffer[n];

    if constexpr (std::is_same<T, __m128i>::value) 
    {
        _mm_storeu_si128((__m128i*)buffer, data);
    }
    else if constexpr (std::is_same<T, __m256i>::value)
    {
        _mm256_storeu_si256((__m256i*)buffer, data);
    }
    else 
    {
        std::memcpy(buffer, &data, sizeof(data));
    }

    print_bits_ext(buffer, n, color);
}
