#include "advent2020.h"

namespace
{
    // Returns a vector of element-wise differences (between sorted adapters)
    std::vector<uint8_t> parse_input(const input_t& in)
    {
        std::vector<uint8_t> out;
        parse_input(in, [&](const sv& line)
        {
            out.push_back(to_int<uint8_t>(line));
        });

        // Insert constant outlets (0 - charging, max+3 - build-in adapter)
        out.push_back(0);
        std::sort(out.begin(), out.end());
        out.push_back(out.back() + 3);

        // Compute element-wise difference
        for (size_t i = 0; i < out.size() - 1; ++i)
        {
            out[i] = out[i + 1] - out[i];
        }
        out.pop_back();

        return out;
    }

    std::pair<size_t, size_t> count_specific_elements(const std::vector<uint8_t>& deltas)
    {
        size_t delta1 = 0;
        size_t delta3 = 0;
        for (auto delta : deltas)
        {
            if (delta == 1)
                ++delta1;
            else if (delta == 3)
                ++delta3;
        }

        return { delta1, delta3 };
    }

    int tribonacci(int n)
    {
        if (n == 0)
            return 0;
        else if (n == 1 || n == 2)
            return 1;
        else
            return tribonacci(n - 1) + tribonacci(n - 2) + tribonacci(n - 3);
    }

    size_t count_variations(const std::vector<uint8_t>& deltas)
    {
        size_t variants = 1;
        int substr_counter = 0;

        for (const uint8_t d : deltas)
        {
            if (d == 1)
            {
                ++substr_counter;
            }
            else if (d == 3)
            {
                // n    = 0  1  2  3  4  5  6  7
                // trib = 0  1  1  2  4  7 13 24
                // for specific groups, we want:
                // "11"   -> 2
                // "111"  -> 4
                // "1111" -> 7
                // etc. therefore, the indexes are shifted by one
                variants *= tribonacci(substr_counter + 1);
                substr_counter = 0;
            }
            else
                assert(false);
        }

        return variants;
    }
}

output_t day10(const input_t& input)
{
    std::vector<uint8_t> element_deltas = parse_input(input);

    const auto& [delta1, delta3] = count_specific_elements(element_deltas);
    size_t variations = count_variations(element_deltas);

    return { (delta1 * delta3), variations };
}

void day10_test()
{
    {
        char text1[] = "16\n10\n15\n5\n1\n11\n7\n19\n6\n12\n4\n";
        input_t input1 { text1, sizeof(text1) };
        std::vector<uint8_t> deltas = parse_input(input1);
        const auto& [delta1, delta3] = count_specific_elements(deltas);
        assert(delta1 == 7 && delta3 == 5);

        size_t variations = count_variations(deltas);
        // printf("variations: %lu\n", variations);
        assert(variations == 8);
    }

    {
        char text2[] = "28\n33\n18\n42\n31\n14\n46\n20\n48\n47\n24\n23\n49\n45\n"
                       "19\n38\n39\n11\n1\n32\n25\n35\n8\n17\n7\n9\n4\n2\n34\n"
                       "10\n3\n";
        input_t input2 { text2, sizeof(text2) };
        std::vector<uint8_t> deltas = parse_input(input2);
        const auto& [delta1, delta3] = count_specific_elements(deltas);
        assert(delta1 == 22 && delta3 == 10);

        size_t variations = count_variations(deltas);
        // printf("variations: %lu\n", variations);
        assert(variations == 19208);
    }
}
