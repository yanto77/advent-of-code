#include "advent2020.h"

namespace
{
    std::vector<int> parse_input(const input_t& input)
    {
        std::vector<int> v;
        v.reserve(1000);

        for (int n = 0, i = 0; i < input.len; i++)
        {
            uint8_t c = input.s[i] - '0';
            if (c < 10) // is digit
            {
                n = 10 * n + c;
            }
            else
            {
                v.emplace_back(n);
                n = 0;
            }
        }

        return v;
    }
}

void day01(input_t input)
{
    // Given input,
    // - part 1:
    //   - find two entries that sum up to 2020
    //   - return product of those entries
    // - part 2:
    //   - find three entries that sum up to 2020
    //   - return product of those entries
    std::vector<int> v = parse_input(input);

    std::sort(v.begin(), v.end());

    // Part 1
    int product1 = -1;
    for (int i: v)
    {
        for (int j: v)
        {
            if (i + j == 2020)
            {
                product1 = i * j;
                break;
            }
        }

        if (product1 != -1)
            break;
    }

    // Part 2
    int product2 = -1;
    for (int i: v)
    {
        for (int j: v)
        {
            for (int k: v)
            {
                if (i + j + k == 2020)
                {
                    product2 = i * j * k;
                    break;
                }
            }

            if (product2 != -1) break;
        }

        if (product2 != -1) break;
    }

    printf("Day 01: %d, %d", product1, product2);
    assert(product1 == 1010884);
    assert(product2 == 253928438);
}
