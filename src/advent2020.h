#pragma once

#include <algorithm>
// #include <array>
// #include <cctype>
// #include <climits>
#include <cmath>
#include <cstdint>
// #include <cstring>
#include <functional>
// #include <set>
#include <string_view>
#include <string>
// #include <unordered_map>
#include <vector>
// #include <x86intrin.h>

#ifdef NDEBUG
#undef NDEBUG
#include <cassert>
#define NDEBUG 1
#else
#include <cassert>
#endif

struct input_t
{
    char* s;
    ssize_t len;
};

void day01(const input_t&);
void day02(const input_t&);
void day03(const input_t&);
void day04(const input_t&);
void day05(const input_t&);
void day06(const input_t&);
void day07(const input_t&);
void day08(const input_t&);
void day09(const input_t&);
void day10(const input_t&);
void day11(const input_t&);
void day12(const input_t&);
void day13(const input_t&);
void day14(const input_t&);
void day15(const input_t&);
void day16(const input_t&);
void day17(const input_t&);
void day18(const input_t&);
void day19(const input_t&);
void day20(const input_t&);
void day21(const input_t&);
void day22(const input_t&);
void day23(const input_t&);
void day24(const input_t&);
void day25(const input_t&);

/// Input helpers

void load_input(input_t& input, const std::string& filename);

void free_input(input_t& input);

void parse_input(const input_t& input, std::function<void(const std::string_view&)> line_cb);

/// Conversion helpers

/// Returns int or (-1) on failure.
int to_int(const std::string_view& sv);

char to_char(const std::string_view& sv);

/// Convert full 6-char hex
/// Expects input in form of "#123abc"
int hex_to_dec(const std::string_view& sv);

/// Bit manipulation helpers

template <typename T>
constexpr void set_bit(T& trg, int bit)
{
    trg |= 1 << (bit);
}

template <typename T>
constexpr bool get_bit(T num, int bit)
{
    return ((num >> bit) & 1) == 1;
}

template <typename T>
constexpr uint8_t count_bits(T n)
{
    uint8_t count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

void print_bits(uint8_t trg);
