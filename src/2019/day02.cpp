#include "setup/all.h"

ADVENT_DAY(2019, 2, 4090689, 7733);

namespace
{
    void run_intcode(std::vector<int32_t>& data)
    {
        size_t ip = 0; // instruction pointer
        while (true)
        {
            switch (data[ip])
            {
            case 1: // sum
                data[data[ip + 3]] = data[data[ip + 1]] + data[data[ip + 2]];
                break;
            case 2: // multiply
                data[data[ip + 3]] = data[data[ip + 1]] * data[data[ip + 2]];
                break;
            case 99: // finish + halt
                return;
            default:
                break;
            }

            ip += 4;
        }
    }

    int32_t run_single_guess(std::vector<int32_t> data, uint8_t verb, uint8_t noun)
    {
        data[1] = verb;
        data[2] = noun;
        run_intcode(data);
        return data[0];
    }

    int32_t run_multiple_guesses(std::vector<int32_t> data, int32_t expected)
    {
        for (uint8_t i = 0; i < 99; ++i)
            for (uint8_t j = 0; j < 99; ++j)
                if (expected == run_single_guess(data, i, j))
                    return 100 * i + j;

        assert(false);
        return 0;
    }
}

void Day_2019_2::run_tests() const
{
    const std::vector<std::string> inputs = {
        "1,0,0,0,99\n",
        "2,3,0,3,99\n",
        "2,4,4,5,99,0\n",
        "1,1,1,4,99,5,6,0,99\n",
        "1,9,10,3,2,3,11,0,99,30,40,50\n",
    };
    const std::vector<std::vector<int32_t>> outputs = {
        { 2, 0, 0, 0, 99 },
        { 2, 3, 0, 6, 99 },
        { 2, 4, 4, 5, 99, 9801 },
        { 30, 1, 1, 4, 2, 5, 6, 0, 99 },
        { 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50 }
    };

    for (size_t idx = 0; idx < inputs.size(); ++idx)
    {
        const std::string& test = inputs[idx];
        sv test1 { test.data(), test.length() };

        std::vector<int32_t> data = to_multi_int<int32_t>(test1);
        // printf("Input : "); print_row(data);

        run_intcode(data);
        // printf("Output: "); print_row(data);

        assert(data == outputs[idx]);
    }
}

output_t Day_2019_2::run_solution(const input_t& input) const
{
    const std::vector<int32_t> data = to_multi_int<int32_t>(get_sv(input));
    size_t part1 = run_single_guess(data, 12, 2);
    size_t part2 = run_multiple_guesses(data, 19690720);

    return {part1, part2};
}
