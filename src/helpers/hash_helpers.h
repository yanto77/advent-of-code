#pragma once
#include <cstddef>
#include <string_view>

// D. J. Bernstein hash function
// Modified from: https://codereview.stackexchange.com/a/85745
size_t djb_hash(const std::string_view& sv);

// Fowler/Noll/Vo (FNV) hash function, variant 1a
// Modified from: https://codereview.stackexchange.com/a/85745
size_t fnv1a_hash(const std::string_view& sv);
