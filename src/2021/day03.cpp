#include "setup/all.h"

ADVENT_DAY(2021, 3, 4147524, 3570354);

namespace
{
    std::vector<uint16_t> get_bitset_data(const input_t& input, size_t line_size)
    {
        std::vector<uint16_t> data {};
        parse_input(input, [&](const sv& line)
        {
            uint16_t value = 0;
            for (int i = 0; i < line_size; i++)
                if (line[i] == '1')
                    set_bit(value, 15-i);

            value = value >> (16 - line_size);
            data.push_back(value);
        });

        return data;
    }

    std::pair<uint16_t, uint16_t> get_part1(const std::vector<uint16_t>& data, size_t line_size)
    {
        const size_t line_count = data.size();

        std::vector<size_t> counters(16, 0);
        for (uint16_t line: data)
        {
            for (int i = 0; i < 16; i++)
                 if (get_bit(line, i))
                    ++counters[i];
        }

        uint16_t most_common = 0;
        for (int i = 0; i < 16; i++)
            if (counters[i] > (line_count / 2))
                set_bit(most_common, i);

        uint16_t least_common = 0;
        for (int i = 0; i < line_size; i++)
            write_bit(least_common, i, !get_bit(most_common, i));

        return { most_common, least_common };
    }

    uint16_t get_part2_impl(std::vector<uint16_t> data, size_t line_size, bool greater_than)
    {
        for (int i = 0; i < line_size; i++)
        {
            if (data.size() == 1)
                break;

            const size_t line_count = data.size();

            size_t counter = 0;
            for (uint16_t line: data)
                if (get_bit(line, line_size - 1 - i))
                    ++counter;

            bool expected_bit = greater_than ?
                counter >= ((float)line_count / 2) :
                counter <  ((float)line_count / 2);

            auto it = std::remove_if(data.begin(), data.end(),
                [&](const uint16_t x) {
                    return get_bit(x, line_size - 1 - i) != expected_bit;
                });

            data.erase(it, data.end());
        }

        return data.front();
    }

    std::pair<uint16_t, uint16_t> get_part2(const std::vector<uint16_t>& data, size_t line_size)
    {
        return {
            get_part2_impl(data, line_size, true),
            get_part2_impl(data, line_size, false)
        };
    }
}

output_t Day_2021_3::run_solution(const input_t& input) const
{
    const std::vector<uint16_t>& data = get_bitset_data(input, 12);
    const auto& [ gamma, epsilon ] = get_part1(data, 12);
    const auto& [ OGR, CSR ] = get_part2(data, 12);

    size_t part1 = gamma * epsilon;
    size_t part2 = OGR * CSR;
    return { part1, part2 };
}

void Day_2021_3::run_tests() const
{
    char text1[] = "00100\n11110\n10110\n10111\n10101\n01111\n"
                   "00111\n11100\n10000\n11001\n00010\n01010\n";
    input_t test1 { text1, sizeof(text1) };

    const std::vector<uint16_t>& data = get_bitset_data(test1, 5);
    const auto& [ gamma, epsilon ] = get_part1(data, 5);
    assert(198 == (gamma * epsilon));

    const auto& [ OGR, CSR ] = get_part2(data, 5);
    assert(230 == (OGR * CSR));
}
