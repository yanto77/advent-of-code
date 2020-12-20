#pragma once
#include <cstdint>

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
