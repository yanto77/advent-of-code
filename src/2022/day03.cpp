#include "setup/all.h"

ADVENT_DAY(2022, 3, 8039, 2510);

namespace 
{
    // Map {a-z, A-Z} to bit indexes (1-52)
    //  - A-Z: 65-90 to 27-52
    //  - a-z: 97-122 to 1-26
    const uint8_t charToIndex[] = {
        27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,
        43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  0,   0,   0,   0,   0,   0,
        1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,
        17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  0,   0,   0,   0,   0,   0,
    };

    // Map bit indexes (1-52) to {a-z, A-Z}
    //  - 1-26 to {a-z}
    //  - 27-52 to {A-Z}
    const uint8_t indexToChar[] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    };

    struct rucksack_t {
        uint64_t left = 0;
        uint64_t right = 0;
    };

    uint8_t get_char_value(uint64_t items)
    {
        int idx = std::countr_zero(items);
        char value = indexToChar[idx];
        return charToIndex[value-65];
    }
}

result_t Day_2022_3::run_solution(str_view input) const
{
    size_t part1 = 0;
    size_t part2 = 0;

    uint8_t lineCount = 0;
    uint64_t groupItem = UINT64_MAX;

    parse_input(input, [&](str_view line)
    {
        str_view leftStr = line.substr(0, line.size() / 2);
        str_view rightStr = line.substr(line.size() / 2, line.size());

        rucksack_t singleSack;
        for (char ch: leftStr) set_bit(singleSack.left, charToIndex[ch-65]-1);
        for (char ch: rightStr) set_bit(singleSack.right, charToIndex[ch-65]-1);

        // Part 1: check duplicates between left/right compartments of single sack.
        {
            uint64_t duplicates = singleSack.left & singleSack.right;
            part1 += get_char_value(duplicates);
        }

        // Part 2: check Elf group of three lines
        {
            groupItem &= (singleSack.left | singleSack.right);
            lineCount++;

            if (lineCount == 3)
            {
                part2 += get_char_value(groupItem);

                lineCount = 0;
                groupItem = UINT64_MAX;
            }
        }
    });

    return { part1, part2 };
}

void Day_2022_3::run_tests() const
{
}
