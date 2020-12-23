#include "advent2020.h"

namespace
{
    std::vector<uint8_t> parse_input(const input_t& in)
    {
        std::vector<uint8_t> out;
        parse_input(in, [&](const sv& line)
        {
            out.push_back(to_int<uint8_t>(line));
        });

        out.push_back(0); // insert jolts for charging outlet
        std::sort(out.begin(), out.end());
        out.push_back(out.back() + 3); // insert jolts for built-in adapter
        return out;
    }

    struct diff_t
    {
        uint32_t diff1 = 0;
        uint32_t diff2 = 0;
        uint32_t diff3 = 0;
    };

    // returns element-wise differences between `data` entities
    std::vector<uint8_t> get_element_wise_deltas(const std::vector<uint8_t>& data)
    {
        std::vector<uint8_t> result;
        result.reserve(data.size() - 1);

        for (size_t i = 1; i < data.size(); ++i)
        {
            result.push_back(data[i] - data[i-1]);
        }

        return result;
    }

    std::pair<size_t, size_t> count_specific_elements(const std::vector<uint8_t>& deltas)
    {
        size_t delta1 = 0;
        size_t delta3 = 0;
        for (auto delta: deltas)
        {
            if (delta == 1) ++delta1;
            else if (delta == 3) ++delta3;
        }

        return {delta1, delta3};
    }

    size_t count_variations(const std::vector<uint8_t>& data)
    {
        print_row(data);
        auto copy = data;

        for (size_t i = 0; i < ((data.size() + 5) % 5); ++i)
        {
            copy.push_back(UINT8_MAX);
        }
        copy.push_back(UINT8_MAX);

        size_t total_variants = 1;

        for (size_t i = 0; i < data.size(); i += 2)
        {
            uint8_t a = copy[i+0];
            uint8_t b = copy[i+1];
            uint8_t c = copy[i+2];
            uint8_t d = copy[i+3];
            uint8_t e = copy[i+4];
            printf("- [a %d to e %d] ", a, e);

            size_t variants = 1;

            // skip single
            if (0 < c-a && c-a < 4) { ++variants; printf(" +skip {b=%d}", b); }
            if (0 < d-b && d-b < 4) { ++variants; printf(" +skip {c=%d}", c); }
            if (0 < e-c && e-c < 4) { ++variants; printf(" +skip {d=%d}", d); }

            // skip multiple
            if (0 < d-a && d-a < 4) { ++variants; printf(" +skip {bc=%d,%d}", b, c); }
            if (0 < e-b && e-b < 4) { ++variants; printf(" +skip {cd=%d,%d}", c, d); }
            if (0 < c-a && c-a < 4 && 0 < e-c && e-c < 4) { ++variants; printf(" +skip {bd=%d,%d}", b, d); }

            printf(" => total: %zu\n", variants);
            total_variants *= variants;
        }

        return total_variants;
    }
}

void day10(const input_t& input)
{
    std::vector<uint8_t> data = parse_input(input);
    std::vector<uint8_t> element_deltas = get_element_wise_deltas(data);

    const auto& [delta1, delta3] = count_specific_elements(element_deltas);
    assert(delta1 * delta3 == 2664);

    // size_t variations = count_variations(element_deltas);
    // printf("variations: %lu\n", variations);
}

void day10_test()
{
    {
        char text1[] = "16\n10\n15\n5\n1\n11\n7\n19\n6\n12\n4\n";
        input_t input1 { text1, 29 };
        std::vector<uint8_t> data = parse_input(input1);
        assert(data[0] == 0 && data[1] == 1 && data[11] == 19 && data[12] == 22);

        const auto& deltas = get_element_wise_deltas(data);
        const auto& [delta1, delta3] = count_specific_elements(deltas);
        assert(delta1 == 7 && delta3 == 5);

        size_t variations = count_variations(data);
        printf("variations: %lu\n", variations);
        assert(variations == 8);
    }

    {
        char text2[] = "28\n33\n18\n42\n31\n14\n46\n20\n48\n47\n24\n23\n49\n45\n"
                       "19\n38\n39\n11\n1\n32\n25\n35\n8\n17\n7\n9\n4\n2\n34\n"
                       "10\n3\n";
        input_t input2 { text2, 87 };
        std::vector<uint8_t> data = parse_input(input2);

        const auto& deltas = get_element_wise_deltas(data);
        const auto& [delta1, delta3] = count_specific_elements(deltas);
        assert(delta1 == 22 && delta3 == 10);

        size_t variations = count_variations(data);
        printf("variations: %lu\n", variations);
        assert(variations == 19208);
    }

    // if (false)
    // {
    //     std::vector<int> deltas = { 1, 2, 3 };

    //     printf("\n");

    //     for (auto a: deltas)
    //     for (auto b: deltas)
    //     for (auto c: deltas)
    //     // for (auto d: asd)
    //     {
    //         // printf("numbs: %d %d %d %d, ", 0, a, a+b, a+b+c);
    //         printf("deltas: %d %d %d, ", a, b, c);

    //         printf("2sum: %d, %d, ", a+b, b+c);
    //         printf("3sum: %d, ", a+b+c);
            
    //         if (a+b < 4)
    //             printf("\n - can skip 2nd adapter");
    //         if (b+c < 4)
    //             printf("\n - can skip 3rd adapter");
    //         if (a+b+c < 4)
    //             printf("\n - can skip 2nd AND 3rd adapter");

    //         // size_t variants = 1;
    //         // if (a + b + c < 4) ++variants;
    //         // if (a + b < 4) ++variants;
    //         // if (b + c < 4) ++variants;
    //         // printf("    variants: base");
    //         // if (d_ab + d_bc + d_cd < 4) printf(" + skip_both");
    //         // if (d_ab + d_bc < 4) printf(" + skip_b");
    //         // if (d_bc + d_cd < 4) printf(" + skip_c");
    //         // printf(" => %zu\n", variants);

    //         printf("\n");
    //     }
    // }
}
