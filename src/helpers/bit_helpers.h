#pragma once
#include <cstdint>
#include <cstdio>

/// Bit manipulation helpers

template <typename T>
constexpr void set_bit(T& trg, size_t bit)
{
    trg |= 1ULL << (bit);
}

template <typename T>
constexpr void reset_bit(T& trg, size_t bit)
{
    trg &= ~(1ULL << (bit));
}

/// write specific `value` at location specified by `bit`
template <typename T>
constexpr void write_bit(T& trg, size_t bit, size_t value)
{
    reset_bit(trg, bit);
    trg |= (value << bit);
}

template <typename T>
constexpr void set_bitmask(T& trg, const T& mask)
{
    trg |= mask;
}

template <typename T>
constexpr void reset_bitmask(T& trg, const T& mask)
{
    trg &= ~mask;
}

inline void reset_bitmask(std::vector<bool>& trg, const std::vector<bool>& mask)
{
    size_t size = trg.size();
    for (size_t i = 0; i < size; ++i)
    {
        trg[i] = (trg[i] & ~mask[i]);
    }
}

template <typename T>
constexpr bool get_bit(T num, size_t bit)
{
    return ((num >> bit) & 1) == 1;
}


template <typename T>
constexpr uint8_t count_bits(T n)
{
    uint8_t count = 0;
    while (n)
    {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

inline uint8_t count_bits(const std::vector<bool>& bits)
{
    uint8_t count = 0;
    for (bool bit: bits)
    {
        count += bit & 1;
    }
    return count;
}
