#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
#include <set>
#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>
#include <x86intrin.h>

#ifdef NDEBUG
#undef NDEBUG
#include <cassert>
#define NDEBUG
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

// Helpers

void parse_input(const input_t& input, std::function<void(const std::string_view&)> line_cb);

int to_int(const std::string_view& sv);

char to_char(const std::string_view& sv);
