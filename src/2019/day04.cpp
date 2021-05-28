#include "setup/all.h"

ADVENT_DAY(2019, 4, 1767, 1192);

namespace
{
    constexpr std::array<uint8_t, 8> get_digits(size_t num)
    {
        std::array<uint8_t, 8> out {};
        for (size_t i = 0; i < 6; ++i)
        {
            out[7-i] = num % 10;
            num /= 10;
        }
        return out;
    }

    std::pair<bool, bool> is_valid(size_t num)
    {
        const auto& digits = get_digits(num);

        bool increasing = 
            (digits[2+0] <= digits[2+1]) &&
            (digits[2+1] <= digits[2+2]) &&
            (digits[2+2] <= digits[2+3]) &&
            (digits[2+3] <= digits[2+4]) &&
            (digits[2+4] <= digits[2+5]);

        if (!increasing)
            return { false, false };

        bool adjacent = 
            (digits[2+0] == digits[2+1]) ||
            (digits[2+1] == digits[2+2]) ||
            (digits[2+2] == digits[2+3]) ||
            (digits[2+3] == digits[2+4]) ||
            (digits[2+4] == digits[2+5]);

        if (!adjacent)
            return { false, false };

        std::array<uint8_t, 10> counter {};
        for (int8_t i = 0; i < 6; ++i)
        {
            ++counter[digits[2+i]];
        }

        // at least one group of exactly 2 digits
        bool small_group = std::any_of(counter.begin(), counter.end(),
            [](uint8_t x) { return x == 2; });

        return { true, small_group };
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
        const auto& [valid1, valid2] = is_valid(num);

        valid1_n += static_cast<size_t>(valid1);
        valid2_n += static_cast<size_t>(valid2);
    }

    return {valid1_n, valid2_n};
}

void Day_2019_4::run_tests() const
{
    std::array<uint8_t, 8> out1 = {0, 0, 1, 2, 3, 4, 5, 6};
    assert(get_digits(123456) == out1);

    assert(is_valid(111111).first == true);
    assert(is_valid(223450).first == false);
    assert(is_valid(123789).first == false);

    assert(is_valid(112233).second == true);
    assert(is_valid(123444).second == false);
    assert(is_valid(111122).second == true);
    assert(is_valid(111123).second == false);


}
