#include "advent2020.h"

constexpr static size_t COL_N = 31;
constexpr static size_t ROW_N = 323;

typedef std::array<std::array<bool, COL_N>, ROW_N> tree_map_t;

namespace
{
    tree_map_t get_tree_map(const input_t& input)
    {
        std::array<std::array<bool, COL_N>, ROW_N> map;
        for (auto& row : map)
        {
            row.fill(false);
        }

        size_t current_row = 0;
        parse_input(input, [&](const sv& line)
        {
            for (size_t c = 0; c < line.size(); ++c)
                if (line[c] == '#')
                    map[current_row][c] = true;

            ++current_row;
        });

        // with given input, there should be 1934 trees.
        return map;
    }

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

output_t day03(const input_t& input)
{
    const tree_map_t& map = get_tree_map(input);

    // Part 1
    size_t part1 = get_encountered_trees(map, { 3, 1 });

    // Part 2
    size_t part2 = part1;
    part2 *= get_encountered_trees(map, vec2i { 1, 1 });
    part2 *= get_encountered_trees(map, vec2i { 5, 1 });
    part2 *= get_encountered_trees(map, vec2i { 7, 1 });
    part2 *= get_encountered_trees(map, vec2i { 1, 2 });

    return { part1, part2 };
}

void day03_test()
{
}
