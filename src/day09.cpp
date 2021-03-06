#include "advent2020.h"

namespace
{
    std::vector<int64_t> parse_input(const input_t& in)
    {
        std::vector<int64_t> out;
        parse_input(in, [&](const sv& line)
        {
            out.push_back(to_int<int64_t>(line));
        });
        return out;
    }

    // Returns number in `input` that is not sum of any two numbers within previous `sliding_n` numbers
    // If not found, returns -1;
    int64_t validate_input(const std::vector<int64_t>& input, size_t sliding_n)
    {
        for (size_t i = sliding_n; i < input.size(); ++i)
        {
            bool found = false;
            for (size_t wx = i - sliding_n; !found && wx < i; ++wx)
            {
                for (size_t wy = i - sliding_n; !found && wy < i && wx != wy; ++wy)
                {
                    if (input[wx] + input[wy] == input[i])
                    {
                        found = true;
                    }
                }
            }

            if (!found)
            {
                return input[i];
            }
        }

        return -1;
    }

    std::pair<size_t, size_t> find_continuous_set(const std::vector<int64_t>& input, int64_t target_sum)
    {
        for (size_t i = 0; i < input.size(); ++i)
        {
            int64_t sum = 0;
            for (size_t j = i; j < input.size(); ++j)
            {
                sum += input[j];

                if (sum == target_sum)
                    return { i, j };
            }
        }

        return { 0, 0 };
    }

    std::pair<int64_t, int64_t> get_min_max(const std::vector<int64_t>& input, size_t start, size_t end)
    {
        int64_t min = INT64_MAX;
        int64_t max = INT64_MIN;

        for (size_t i = start; i < end; ++i)
        {
            if (input[i] > max) max = input[i];
            if (input[i] < min) min = input[i];
        }

        return { min, max };
    }
}

output_t day09(const input_t& input)
{
    std::vector<int64_t> data = parse_input(input);
    size_t invalid_num = validate_input(data, 25);

    const auto& [start, end] = find_continuous_set(data, invalid_num);
    const auto& [min, max] = get_min_max(data, start, end);

    return { invalid_num, static_cast<size_t>(min+max) };
}

void day09_test()
{
    char text[] = "35\n"
                  "20\n"
                  "15\n"
                  "25\n"
                  "47\n"
                  "40\n"
                  "62\n"
                  "55\n"
                  "65\n"
                  "95\n"
                  "102\n"
                  "117\n"
                  "150\n"
                  "182\n"
                  "127\n"
                  "219\n"
                  "299\n"
                  "277\n"
                  "309\n"
                  "576\n";
    input_t input { text, sizeof(text) };
    std::vector<int64_t> data = parse_input(input);
    assert(data[0] == 35);
    assert(data[19] == 576);
    assert(validate_input(data, 5) == 127);

    const auto& [start, end] = find_continuous_set(data, 127);
    const auto& [min, max] = get_min_max(data, start, end);
    assert(start == 2 && end == 5);
    assert(min == 15 && max == 47);
}
