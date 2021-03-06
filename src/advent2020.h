#pragma once

#include <3rd-party/ctre.hpp> // https://github.com/hanickadot/compile-time-regular-expressions
#include <algorithm>
#include <array>
#include <bitset>
#include <cctype>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <deque>
#include <functional>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>
#include <x86intrin.h>

using namespace std::chrono_literals;

#ifdef NDEBUG
    #undef NDEBUG
    #include <cassert>
    #define NDEBUG 1
#else
    #include <cassert>
#endif

#include "helpers/array.h"
#include "helpers/bit.h"
#include "helpers/conv.h"
#include "helpers/hash.h"
#include "helpers/input.h"
#include "helpers/pow.h"
#include "helpers/print.h"
#include "helpers/string.h"
#include "helpers/vec2.h"
#include "helpers/vec3.h"

#include "setup/days.h"
