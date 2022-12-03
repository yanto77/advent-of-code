#pragma once

// std includes
#include <bitset>
#include <cstdint>
#include <cstddef>
#include <queue>
#include <string_view>
#include <unordered_map>

// 3rd-party includes
#include <fmt/core.h>
#include <fmt/ranges.h>

// custom helper includes
#include <helpers/array.h>
#include <helpers/bit.h>
#include <helpers/conv.h>
#include <helpers/print.h>
#include <helpers/string.h>
#include <helpers/vec2.h>
#include <helpers/vec3.h>
#include <helpers/math.h>
#include <helpers/num.h>

// ADVENT_DAY() macro
#include <setup/factory.h>

// Include assert() for all builds (incl. Release)
#ifdef NDEBUG
    #undef NDEBUG
    #include <cassert>
    #define NDEBUG 1
#else
    #include <cassert>
#endif