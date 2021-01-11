#include "advent2020.h"
#include <map>

constexpr uint8_t MASK_BIT_COUNT = 36;

typedef std::map<uint64_t, uint64_t> memory_t;
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

namespace
{
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

        // Apply setX
        std::vector<uint8_t> dst_idx; // bit destination indices
        dst_idx.reserve(MASK_BIT_COUNT / 2);
        for (uint8_t bit = 0; bit < MASK_BIT_COUNT; ++bit)
        {
            if (get_bit(mask.setX, bit))
            {
                dst_idx.push_back(bit);
            }
        }

        // For all possible permutations of given bits in mask.setX:
        size_t bit_count = dst_idx.size();
        for (int64_t perm = 0; perm < binpow(2, bit_count); ++perm)
        {
            // Overwrite mask's bits with bits of current permutation
            for (uint8_t bit = 0; bit < bit_count; ++bit)
                write_bit(addr, dst_idx[bit], get_bit(perm, bit));

            memory[addr] = value;
        }
    }

    size_t evaluate(const input_t& input, bool part1 = true)
    {
        memory_t memory;
        mask_t mask;

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
