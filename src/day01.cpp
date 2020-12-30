#include "advent2020.h"

output_t day01(const input_t& input)
{
    // Given input,
    // - part 1:
    //   - find two entries that sum up to 2020
    //   - return product of those entries
    // - part 2:
    //   - find three entries that sum up to 2020
    //   - return product of those entries
    std::vector<int> v;
    v.reserve(1000);
    parse_input(input, [&](const std::string_view& line)
    {
        v.emplace_back(to_int<int>(line));
    });

    std::sort(v.begin(), v.end());

    // Part 1
    size_t product1 = 0;
    for (int i : v)
    {
        for (int j : v)
        {
            if (i + j == 2020)
            {
                product1 = i * j;
                break;
            }
        }

        if (product1 != 0)
            break;
    }

    // Part 2
    size_t product2 = 0;
    for (int i : v)
    {
        for (int j : v)
        {
            for (int k : v)
            {
                if (i + j + k == 2020)
                {
                    product2 = i * j * k;
                    break;
                }
            }

            if (product2 != 0)
                break;
        }

        if (product2 != 0)
            break;
    }

    return { product1, product2 };
}

void day01_test()
{
}
