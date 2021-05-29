#pragma once
#include <array>
#include <cstdint>

template<size_t N>
constexpr std::array<uint8_t, N> get_digits(size_t input)
{
    std::array<uint8_t, N> out {};
    for (size_t i = 0; i < N; ++i)
    {
        out[N-1-i] = input % 10;
        input /= 10;
    }
    return out;
}
