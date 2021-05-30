#include "setup/all.h"
#include "intcode.h"

ADVENT_DAY(2019, 8, 2080, 1);

namespace
{
    constexpr size_t W = 25;
    constexpr size_t H = 6;

    struct layer_t 
    {
        std::bitset<(W*H)> values; // contains values {0, 1} from input
        std::bitset<(W*H)> mask; // contains values {2} from input (and zero for {0, 1})
    };
    typedef std::vector<layer_t> image_t;

    [[maybe_unused]]
    void print_layer(const layer_t& layer)
    {
        fmt::print("layer:\n");
        for (int8_t h = H - 1; h >= 0; --h)
        {
            for (int8_t w = W - 1; w >= 0; --w)
            {
                fmt::print("{}", (layer.values[(h)*W + w]) ? '*' : ' ');
                // fmt::print("{}", (layer.values[(h)*W + w]) ? '1' : '0');
            }
            fmt::print("\n");
        }
        fmt::print("\n");
    }

    image_t parse_image(const input_t& input)
    {
        image_t image;
        for (size_t i = 0; i < (input.len - 1); ++i)
        {
            const size_t layer = i / (W*H);
            if (layer >= image.size())
            {
                image.emplace_back(layer_t {});
            }

            const size_t pos = i % (W*H);
            const char value = input.s[i];
            if (value == '1')
                image[layer].values.set((W*H)-1 - pos);
            else if (value == '2')
                image[layer].mask.set((W*H)-1 - pos);
        }

        return image;
    }

    size_t get_part1(const image_t& image)
    {
        size_t min_zeros = SIZE_MAX;
        size_t part1 = 0;
        for (const layer_t& layer: image)
        {
            size_t ones = layer.values.count();
            size_t twos = layer.mask.count();
            size_t zeros = (W*H) - ones - twos;
            if (zeros < min_zeros)
            {
                min_zeros = zeros;
                part1 = ones * twos;
            }
        }
        return part1;
    }

    layer_t decode_image(const image_t& image)
    {
        layer_t final_image {};
        for (auto it = image.rbegin(); it != image.rend(); ++it)
        {
            write_bitmask(final_image.values, ~(it->mask), it->values);
        }

        return final_image;
    }
}

output_t Day_2019_8::run_solution(const input_t& input) const
{
    image_t image = parse_image(input);
    size_t part1 = get_part1(image);
    layer_t final_image = decode_image(image);


    const std::bitset<(W*H)> expected { 
        "0110010010111000110010001"
        "1001010010100101001010001"
        "1001010010100101000001010"
        "1111010010111001000000100"
        "1001010010101001001000100"
        "1001001100100100110000100"
    };
    assert(final_image.values == expected);
    // print_layer(final_image); // spells AURCY

    return {part1, 1};
}

void Day_2019_8::run_tests() const
{
    std::bitset<4> in {};
    std::vector<std::pair<std::bitset<4>, std::bitset<4>>> image = 
    {
        { 0b0000, 0b0111 },
        { 0b1100, 0b0011 },
        { 0b0010, 0b1101 },
        { 0b0000, 0b0000 },
    };

    for (auto it = image.rbegin(); it != image.rend(); ++it)
    {
        auto value = it->first;
        auto mask = ~(it->second);
        write_bitmask(in, mask, value);
    }

    assert(in == 0b0110);
    
}
