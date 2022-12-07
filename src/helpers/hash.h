#pragma once
#include <cstddef>
#include <deque>
#include <string_view>

// D. J. Bernstein hash function
// Modified from: https://codereview.stackexchange.com/a/85745
inline uint64_t djb_hash(str_view str_view)
{
    uint64_t hash = 5381;
    for (auto ch : str_view)
    {
        hash = 33 * hash ^ static_cast<uint64_t>(ch);
    }
    return hash;
}

// D. J. Bernstein hash function
// Modified from: https://codereview.stackexchange.com/a/85745
inline uint64_t djb_hash(const std::deque<int>& deque)
{
    uint64_t hash = 5381;
    for (auto ch : deque)
    {
        hash = 33 * hash ^ static_cast<uint64_t>(ch);
    }
    return hash;
}

// Fowler/Noll/Vo (FNV) hash function, variant 1a
// Modified from: https://codereview.stackexchange.com/a/85745
inline uint64_t fnv1a_hash(str_view str_view)
{
    uint64_t hash = 0x811c9dc5;
    for (auto ch : str_view)
    {
        hash ^= static_cast<unsigned char>(ch);
        hash *= 0x01000193;
    }
    return hash;
}