#include "hash_helpers.h"

// D. J. Bernstein hash function
// Modified from: https://codereview.stackexchange.com/a/85745
size_t djb_hash(const std::string_view& sv)
{
    size_t hash = 5381;
    for (auto ch : sv)
    {
        hash = 33 * hash ^ static_cast<unsigned char>(ch);
    }
    return hash;
}

// Fowler/Noll/Vo (FNV) hash function, variant 1a
// Modified from: https://codereview.stackexchange.com/a/85745
size_t fnv1a_hash(const std::string_view& sv)
{
    size_t hash = 0x811c9dc5;
    for (auto ch : sv)
    {
        hash ^= static_cast<unsigned char>(ch);
        hash *= 0x01000193;
    }
    return hash;
}
