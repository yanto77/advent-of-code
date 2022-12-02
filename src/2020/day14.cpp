#include "setup/all.h"

ADVENT_DAY(2020, 14, 14954914379452, 3415488160714);

namespace
{
    constexpr uint8_t MASK_BIT_COUNT = 36;

    typedef std::unordered_map<uint64_t, uint64_t> memory_t;
    struct mask_t
    {
        uint64_t set0 = 0;
        uint64_t set1 = 0;
        uint64_t setX = 0;

        void parse_from(str_view input)
        {
            set0 = set1 = setX = 0;

            for (size_t i = 0; i < MASK_BIT_COUNT; ++i)
            {
                if (input[i] == '0') set_bit(set0, 35-i);
                else if (input[i] == '1') set_bit(set1, 35-i);
                else if (input[i] == 'X') set_bit(setX, 35-i);
            }
        }
    };

    void apply_pt1(memory_t& memory, const mask_t& mask, uint64_t addr, uint64_t value)
    {
        set_bitmask(value, mask.set1);
        reset_bitmask(value, mask.set0);
        memory[addr] = value;
    }

    void apply_pt2(memory_t& memory, const mask_t& mask, uint64_t addr, uint64_t value)
    {
        // NB, set0 is ignored.

        // Apply set0
        set_bitmask(addr, mask.set1);

        // Iterate over all the possible combinations of floating bits.
        // Ref: https://github.com/Scrumplesplunge/aoc2020/blob/master/src/day14.c
        size_t floating_values = 0;
        do
        {
            memory[addr ^ floating_values] = value;
            floating_values = (floating_values + 1 + ~mask.setX) & mask.setX;
        } while (floating_values);
    }

    size_t evaluate(str_view input, bool part1 = true)
    {
        mask_t mask;

        memory_t memory;
        memory.reserve(part1 ? 1000 : 100000);

        parse_input(input, [&](str_view line)
        {
            const auto& [type, arg] = split_single(line, " = ");
            if (type.starts_with("mask"))
            {
                mask.parse_from(arg);
            }
            else
            {
                uint64_t addr = to_int<uint64_t>(str_view { &type[4], type.size() - 5 });
                uint64_t value = to_int<uint64_t>(arg);

                if (part1)
                    apply_pt1(memory, mask, addr, value);
                else
                    apply_pt2(memory, mask, addr, value);
            }
        });

        size_t sum = 0;
        for (const auto& [key, value] : memory)
        {
            sum += value;
        }
        return sum;
    }
}

result_t Day_2020_14::run_solution(str_view input) const
{
    size_t part1 = evaluate(input, true);
    size_t part2 = evaluate(input, false);
    return { part1, part2 };
}

void Day_2020_14::run_tests() const
{
    char input1[] = "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X\n"
                    "mem[8] = 11\n"
                    "mem[7] = 101\n"
                    "mem[8] = 0\n";
    size_t test1_out = evaluate(input1, true);
    assert(test1_out == 165);

    char input2[] = "mask = 000000000000000000000000000000X1001X\n"
                    "mem[42] = 100\n"
                    "mask = 00000000000000000000000000000000X0XX\n"
                    "mem[26] = 1\n";
    size_t test2_out = evaluate(input2, false);
    assert(test2_out == 208);
}
