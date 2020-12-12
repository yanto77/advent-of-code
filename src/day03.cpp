#include "advent2020.h"

constexpr static size_t COL_N = 31;
constexpr static size_t ROW_N = 323;

typedef std::array<std::array<bool, COL_N>, ROW_N> tree_map_t;

namespace
{
    tree_map_t get_tree_map(const input_t& input)
    {
        std::array<std::array<bool, COL_N>, ROW_N> map;
        for (auto& row: map)
        {
            row.fill(false);
        }

        size_t current_row = 0;
        parse_input(input, [&](const std::string_view& line)
        {
            for (size_t c = 0; c < line.size(); ++c)
                if (line[c] == '#')
                    map[current_row][c] = true;

            ++current_row;
        });

        // with given input, there should be 1934 trees.
        return map;
    }

    struct vec2i
    {
        int x;
        int y;

        vec2i& operator+=(const vec2i& other)
        {
            x = x + other.x;
            y = y + other.y;
            return *this;
        }
    };

    size_t get_encountered_trees(const tree_map_t& map, const vec2i& slope)
    {
        vec2i pos { 0, 0 };

        int trees_encountered = 0;
        while (pos.y < ROW_N)
        {
            // printf("current pos: col %d, row %d\n", pos.y, pos.x);
            if (map[pos.y][pos.x % COL_N])
                ++trees_encountered;

            pos += slope;
        }

        return trees_encountered;
    }
}

void day03(const input_t& input)
{
    static const std::array<vec2i, 5> slopes =
    {
        vec2i{1, 1},
        vec2i{3, 1},
        vec2i{5, 1},
        vec2i{7, 1},
        vec2i{1, 2},
    };

    const tree_map_t& map = get_tree_map(input);

    size_t result_product = 1; // Part 2


    for (const vec2i& slope: slopes)
    {
        size_t trees = get_encountered_trees(map, slope);
        // printf("slope (%d, %d) encounters %zu trees\n", slope.x, slope.y, trees);
        result_product *= trees;
    }

    printf("%zu", result_product);
    assert(result_product == 3093068400);
}
