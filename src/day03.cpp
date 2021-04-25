#include "advent2020.h"

static constexpr size_t COL_N = 31;
static constexpr size_t ROW_N = 323;

typedef std::array<uint32_t, ROW_N> tree_map_t;

namespace
{
    tree_map_t get_tree_map(const input_t& input)
    {
        tree_map_t map {};

        size_t row = 0;
        parse_input(input, [&](const sv& line)
        {
            for (size_t c = 0; c < line.size(); ++c)
                if (line[c] == '#')
                    set_bit(map[row], c);

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

output_t day03(const input_t& input)
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

void day03_test()
{
}
