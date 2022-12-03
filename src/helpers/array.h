#pragma once
#include <array>

/// Get index of the value appearing in the array first time
template <typename T, size_t N>
ssize_t get_index_of(const std::array<T, N>& input, const T value)
{
    for (size_t idx = 0; idx < input.size(); ++idx)
        if (input[idx] == value)
            return idx;

    return -1;
}
