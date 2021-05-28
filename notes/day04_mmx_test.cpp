#include "setup/all.h"

namespace
{
    __m64 pack(std::array<uint8_t, 8> in)
    {
        return *(reinterpret_cast<__m64*>(&in));
    }

    std::array<uint8_t, 8> unpack(__m64 in)
    {
        return *(reinterpret_cast<std::array<uint8_t, 8>*>(&in));
    }

    uint8_t get_packed_nth(__m64 data, uint8_t n)
    {
        uint8_t* arr = reinterpret_cast<uint8_t*>(&data);
        return arr[n];
    }

    void set_packed_nth(__m64& data, uint8_t n, uint8_t value)
    {
        uint8_t* arr = reinterpret_cast<uint8_t*>(&data);
        arr[n] = value;
    }

    __m64 get_digits(size_t num)
    {
        __m64 out {};
        for (uint8_t i = 0; i < 6; ++i)
        {
            set_packed_nth(out, 7-i, num % 10);
            num /= 10;
        }
        return out;
    }

    bool is_valid(size_t num)
    {
        __m64 packed = get_digits(num);
        __m64 shifted = packed << 8;

        __m64 cmpgt = _mm_cmpgt_pi8(packed, shifted);
        __m64 cmpeq = _mm_cmpeq_pi8(packed, shifted);
        __m64 comb = cmpgt | cmpeq;

        bool increasing = (_m_to_int64(comb) == -1);
        if (!increasing)
            return false;

        const __m64 full_mask = pack({0, 0, 255, 255, 255, 255, 255, 255});
        __m64 cmpeq2 = _mm_and_si64(cmpeq, full_mask);

        bool adjacent = (_m_to_int64(cmpeq2) != 0);
        return adjacent;
    }
}

output_t Day_2019_4::run_solution(const input_t& /* input */) const
{
    size_t min = 145852;
    size_t max = 616942;

    size_t valid1_n = 0;
    size_t valid2_n = 0;

    for (size_t num = min; num <= max; ++num)
    {
        valid1_n += is_valid(num);
    }
    
    return {valid1_n, valid2_n};
}