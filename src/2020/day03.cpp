#include "setup/all.h"

ADVENT_DAY(2020, 3, 268, 3093068400);

static constexpr size_t COL_N = 31;
static constexpr size_t ROW_N = 323;

typedef std::array<uint32_t, ROW_N> tree_map_t;

namespace
{
    [[maybe_unused]]
    uint32_t convert_basic(const char* input)
    {
        std::bitset<32> output { 0 };
        for (size_t c = 0; c < 32; ++c)
            if (input[c] == '#')
                output.set(c);

        return static_cast<uint32_t>(output.to_ulong());
    }

    [[maybe_unused]]
    uint32_t convert_avx2(const char* input)
    {
        const auto* ptr = reinterpret_cast<const __m256i *>(input);

        // Load unaligned signed integer from memory address
        //
        // Input:  "#.#\n"
        // Output: 00100011 00101110 00100011 00001010
        __m256i v = _mm256_loadu_si256(ptr);

        // Broadcast 8-bit integer (char) to all elements of dst
        //
        // Output: 00100011 00100011 00100011 00100011
        __m256i mask = _mm256_set1_epi8('#');

        // Compare packed 8-bit integers for equality (not bits directly!)
        //
        // a:   00100011 00101110 00100011 00001010
        // b :  00100011 00100011 00100011 00100011
        // out: 11111111 00000000 11111111 00000000
        v = _mm256_cmpeq_epi8(v, mask);

        // Reduce 256-bit element to 32-bit element, by taking the most
        // significant bit of each 8-bit element
        //
        // In : 11111111 00000000 11111111 00000000
        // Out: 1010
        return _mm256_movemask_epi8(v);
    }

    tree_map_t get_tree_map(str_view input)
    {
        tree_map_t map {};

        size_t row = 0;
        parse_input(input, [&](str_view line)
        {
            map[row] = convert_avx2(line.data());
            ++row;
        });

        // with given input, there should be 1934 trees.
        return map;
    }

    size_t get_encountered_trees(const tree_map_t& map, const vec2i& slope)
    {
        vec2i pos { 0, 0 };

        int trees = 0;
        while (pos.y < ROW_N)
        {
            if (get_bit(map[pos.y], pos.x % COL_N))
                ++trees;

            pos += slope;
        }

        return trees;
    }
}

result_t Day_2020_3::run_solution(str_view input) const
{
    const tree_map_t& map = get_tree_map(input);

    size_t part1 = get_encountered_trees(map, { 3, 1 });
    size_t part2 = part1;
    part2 *= get_encountered_trees(map, vec2i { 1, 1 });
    part2 *= get_encountered_trees(map, vec2i { 5, 1 });
    part2 *= get_encountered_trees(map, vec2i { 7, 1 });
    part2 *= get_encountered_trees(map, vec2i { 1, 2 });

    return { part1, part2 };
}

void Day_2020_3::run_tests() const
{
    constexpr bool PRINT_DEBUG = false;
    if constexpr (PRINT_DEBUG) // AVX2 print debug test
    {
        const char* data = "#.#..#...#..#....#.......####.#\n";
        auto* data_ptr = reinterpret_cast<const __m256i *>(data);

        __m256i v = _mm256_loadu_si256(data_ptr);
        printf("input:"); print_bits(v); printf("\n");

        __m256i mask = _mm256_set1_epi8('#');
        printf("mask :"); print_bits(mask); printf("\n");

        v = _mm256_cmpeq_epi8(v, mask);
        printf("cmpeq:"); print_bits(v); printf("\n");

        uint32_t out = _mm256_movemask_epi8(v);
        printf("out  :"); print_bits(out); printf("\n");
        printf("\n");
    }
}
