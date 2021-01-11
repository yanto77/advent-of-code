#include "advent2020.h"
#include <unordered_map>

namespace
{
    constexpr uint8_t MASK_BIT_COUNT = 36;

    typedef std::unordered_map<uint64_t, uint64_t> memory_t;
    struct mask_t
    {
        uint64_t set0 = 0;
        uint64_t set1 = 0;
        uint64_t setX = 0;

        void parse_from(const sv& input)
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

    [[maybe_unused]]
    void custom_print_bits(uint64_t trg, bool color = false)
    {
        for (int i = MASK_BIT_COUNT; i >= 0; --i)
        {
            if (color)
                printf(get_bit(trg, i) ? COLOR_GREEN() "1" : COLOR_CYAN() "0");
            else
                printf(get_bit(trg, i) ? "1" : "0");
            printf(COLOR_RESET());
        }
    }

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

    size_t evaluate(const input_t& input, bool part1 = true)
    {
        mask_t mask;

        memory_t memory;
        memory.reserve(part1 ? 1000 : 100000);

        parse_input(input, [&](const sv& line)
        {
            const auto& [type, arg] = split_single(line, " = ");
            if (type.starts_with("mask"))
            {
                mask.parse_from(arg);
            }
            else
            {
                uint64_t addr = to_int<uint64_t>(sv { &type[4], type.size() - 5 });
                uint64_t value = to_int<uint64_t>(arg);

                if (part1)
                    apply_pt1(memory, mask, addr, value);
                else
                    apply_pt2(memory, mask, addr, value);
            }
        });

        size_t sum = 0;
        for (const auto& [key, value]: memory) { sum += value; }
        return sum;
    }
}

output_t day14(const input_t& input)
{
    size_t part1 = evaluate(input, true);
    size_t part2 = evaluate(input, false);
    return { part1, part2 };
}

void day14_test()
{
    char input1[81] =
        "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X\n"
        "mem[8] = 11\n"
        "mem[7] = 101\n"
        "mem[8] = 0\n";
    input_t test1 { input1, 81 };
    size_t test1_out = evaluate(test1, true);
    assert(test1_out == 165);

    char input2[115] =
        "mask = 000000000000000000000000000000X1001X\n"
        "mem[42] = 100\n"
        "mask = 00000000000000000000000000000000X0XX\n"
        "mem[26] = 1\n";
    input_t test2 { input2, 115 };
    size_t test2_out = evaluate(test2, false);
    assert(test2_out == 208);
}
