#include "setup/all.h"
#include <optional>

ADVENT_DAY(2022, 25, 36067808580527, 1);

namespace 
{
    constexpr size_t N_MAX = 20;
    constexpr int64_t S_POWS[N_MAX] = {
        19073486328125, 3814697265625, 762939453125, 152587890625, 30517578125, 
        6103515625, 1220703125, 244140625, 48828125, 9765625, 1953125, 390625, 
        78125, 15625, 3125, 625, 125, 25, 5, 1
    };
    constexpr int8_t S_CHAR_TO_DEC[] = {-1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2};
    constexpr char S_DEC_TO_CHAR[] = {'=', '-', '0', '1', '2'};
    
    constexpr int64_t from_snafu(str_view input)
    {
        std::array<int8_t, N_MAX> nums {};
        size_t idx = N_MAX - input.size();
        for (char ch: input)
        {
            nums[idx++] = S_CHAR_TO_DEC[ch-45];
        }

        int64_t out = 0;
        for (size_t i = 0; i < N_MAX; i++)
        {
            out += (S_POWS[i] * nums[i]);
        }

        return out;
    }

    [[maybe_unused]]
    std::string to_snafu(int64_t target)
    {
        std::array<int64_t, N_MAX> divs {};
        for (size_t i = 0; i < N_MAX; i++)
        {
            divs[i] = target / S_POWS[i];
            target -= divs[i] * S_POWS[i];
        }

        for (size_t i = 0; i < N_MAX - 1; i++) // smallest to largest
        {
            int64_t div = divs[N_MAX-1-i];
            if (div > 2)
            {
                divs[N_MAX-1-i] = (((div + 2) % 5) - 2);
                divs[N_MAX-2-i] += 1;
            }
        }

        std::string out = "00000000000000000000";
        for (size_t i = 0; i < N_MAX; i++)
        {
            out[i] = S_DEC_TO_CHAR[divs[i] + 2];
        }

        // fmt::print("out: {}\n", out);
        return out;
    }
}

result_t Day_2022_25::run_solution(str_view input) const
{
    size_t part1 = 0;
    size_t part2 = 1;

    for_each_split(input, '\n', [&](str_view line)
    {
        part1 += from_snafu(line);
    });

    assert(to_snafu(part1) == std::string("2-212-2---=00-1--102"));
    return { part1, part2 };
}

void Day_2022_25::run_tests() const
{
    static_assert(from_snafu("1121-1110-1=0") == 314159265);
    assert(to_snafu(314159265) == std::string("00000001121-1110-1=0"));
}
