#pragma once

template <typename T, size_t N>
T get_max(const std::array<T, N>& input)
{
    T max = 0;
    for (const T el : input)
        if (el > max)
            max = el;

    return max;
}

template <typename T, size_t N>
size_t get_count(const std::array<T, N>& input, const T value)
{
    T count = 0;
    for (const T& el : input)
        if (el == value)
            ++count;

    return count;
}

template <typename T, size_t N>
ssize_t get_index_of(const std::array<T, N>& input, const T value)
{
    for (size_t idx = 0; idx < input.size(); ++idx)
        if (input[idx] == value)
            return idx;

    return -1;
}
