#include "advent2020.h"

namespace
{
    enum side_t { TOP, RIGHT, BOTTOM, LEFT };
    constexpr std::array<side_t, 4> SIDES = { TOP, RIGHT, BOTTOM, LEFT };
    constexpr std::array<std::array<int, 4>, 8> ORIENTATIONS = {
        std::array<int, 4>{ 0, 1, 2, 3 }, // rot0
        std::array<int, 4>{ 3, 0, 1, 2 }, // rot90
        std::array<int, 4>{ 2, 3, 0, 1 }, // rot180
        std::array<int, 4>{ 1, 2, 3, 0 }, // rot270
        std::array<int, 4>{ 0, 3, 2, 1 }, // flip0
        std::array<int, 4>{ 1, 0, 3, 2 }, // flip90
        std::array<int, 4>{ 2, 1, 0, 3 }, // flip180
        std::array<int, 4>{ 3, 2, 1, 0 }, // flip270
    };

    struct tile_t
    {
        // matrix of 10 x 10 pixels
        size_t id = 0;
        std::array<std::array<bool, 10>, 4> sides = {};
        std::array<std::array<bool, 8>, 8> inner = {};
        std::array<tile_t*, 4> side_tiles = {};
        bool visited = false;

        [[maybe_unused]]
        void print_side(side_t s) const
        {
            printf("%d: ", s);
            for (int x = 0; x < sides[s].size(); ++x)
                printf("%c", sides[s][x] ? '#' : '.');
            printf("\n");
        }

        [[maybe_unused]]
        void print() const
        {
            printf("ID: %zu ", id);

            // print_side(TOP);
            // print_side(RIGHT);
            // print_side(BOTTOM);
            // print_side(LEFT);

            printf(" [TOP   : %4zu]", side_tiles[TOP] ? side_tiles[TOP]->id : 0);
            printf(" [RIGHT : %4zu]", side_tiles[RIGHT] ? side_tiles[RIGHT]->id : 0);
            printf(" [BOTTOM: %4zu]", side_tiles[BOTTOM] ? side_tiles[BOTTOM]->id : 0);
            printf(" [LEFT  : %4zu]\n", side_tiles[LEFT] ? side_tiles[LEFT]->id : 0);

            // printf("Inner:\n");
            // for (int y = 0; y < inner.size(); ++y)
            // {
            //     for (int x = 0; x < inner[0].size(); ++x)
            //     {
            //         printf("%c", inner[y][x] ? '#' : '.');
            //     }
            //     printf("\n");
            // }
            // printf("\n");
        }
    };

    std::vector<tile_t> parse_tiles(const input_t& input)
    {
        std::vector<tile_t> tiles;

        enum { ID, MAP } state = ID;
        int row = 0;
        tile_t tile;
        parse_input(input, [&](const sv& line)
        {
            if (state == ID)
            {
                // assumes IDs are 4-num
                tile = {};
                tile.id = to_int<int>(sv{ line.data() + 5, 4 });

                state = MAP;
                row = 0;
            }
            else if (state == MAP)
            {
                if (!line.empty())
                {
                    for (int col = 0; col < 10; ++col)
                    {
                        const bool value = (line[col] == '#');

                        if (col != 0 && col != 9 && row != 0 && row != 9)
                            tile.inner[row-1][col-1] = value;

                        if (col == 0)
                            tile.sides[LEFT][row] = value;
                        else if (col == 9)
                            tile.sides[RIGHT][row] = value;

                        if (row == 0)
                            tile.sides[TOP][col] = value;
                        else if (row == 9)
                            tile.sides[BOTTOM][col] = value;
                    }

                    ++row;
                }
                else
                {
                    tiles.push_back(tile);
                    state = ID;
                }
            }
        });

        return tiles;
    }

    void check_two_tiles(std::vector<tile_t>& tiles, size_t t1, size_t t2)
    {
        for (const side_t s1: SIDES)
        {
            if (tiles[t1].side_tiles[s1] != nullptr)
                continue;

            for (const side_t s2: SIDES)
            {
                if (tiles[t2].side_tiles[s2] != nullptr)
                    continue;

                const auto& side1 = tiles[t1].sides[s1];
                const auto& side2 = tiles[t2].sides[s2];

                std::array<bool, 10> side1r;
                std::reverse_copy(std::begin(side1), std::end(side1), std::begin(side1r));

                if (side1 == side2 || side1r == side2)
                {
                    tiles[t1].side_tiles[s1] = &tiles[t2];
                    tiles[t2].side_tiles[s2] = &tiles[t1];
                    return;
                }
            }
        }
    }

    std::array<tile_t*, 4> get_corners(std::vector<tile_t>& tiles)
    {
        for (size_t t1 = 0; t1 < tiles.size(); ++t1)
        {
            for (size_t t2 = 0; t2 < tiles.size(); ++t2)
            {
                if (t1 <= t2) // skip duplicate and unnecessary checks
                    continue;

                check_two_tiles(tiles, t1, t2);
            }
        }

        std::array<tile_t*, 4> corners = { };
        size_t idx = 0;
        for (size_t t = 0; t < tiles.size(); ++t)
        {
            size_t adjacent_n = 0;

            for (auto side : SIDES)
                if (tiles[t].side_tiles[side] != nullptr)
                    ++adjacent_n;

            if (adjacent_n == 2)
                corners[idx++] = &tiles[t];
        }

        return corners;
    }

    void print_tile_map_ids(tile_t* top_left_corner)
    {
        tile_t* row = top_left_corner;
        while (row != nullptr)
        {
            tile_t* col = row;
            while (col != nullptr)
            {
                printf(" %zu ", col->id);
                col = col->side_tiles[RIGHT];
            }

            printf("\n");
            row = row->side_tiles[BOTTOM];
        }
    }

    tile_t* get_left_top_corner(const std::array<tile_t*, 4>& corners)
    {
        for (auto corner: corners)
            if (corner->side_tiles[TOP] == nullptr && corner->side_tiles[RIGHT] != nullptr)
                return corner;

        return nullptr;
    }

    void orient_tiles(const std::array<tile_t*, 4>& corners)
    {
        tile_t* top_left_corner = get_left_top_corner(corners);

        size_t counter = 0;
        tile_t* tile = top_left_corner;
        side_t dir = RIGHT;

        while (true)
        {
            tile_t* next_tile = tile->side_tiles[dir];
            if (next_tile != nullptr && !next_tile->visited)
            {
                tile->visited = true;

                bool match = false; // safe-guard; there MUST be one orientation that will match
                for (const auto& o: ORIENTATIONS)
                {
                    std::array<tile_t*, 4> sides;
                    for (size_t i = 0; i < 4; ++i)
                    {
                        sides[i] = next_tile->side_tiles[o[i]];
                    }

                    const side_t op_dir = static_cast<side_t>((dir + 2) % 4); // opposite direction
                    const side_t hook_dir = static_cast<side_t>((dir + 3) % 4); // hook direction

                    const tile_t* hook1 = tile->side_tiles[hook_dir];
                    const tile_t* hook2 = hook1 ? hook1->side_tiles[dir] : nullptr;

                    if (tile == sides[op_dir] && hook2 == sides[hook_dir])
                    {
                        next_tile->side_tiles = sides;
                        match = true;
                    }
                }

                if (!match)
                    printf(COLOR_RED() " - didn't find a match!!\n" COLOR_RESET());

                tile = next_tile;
                counter = 0;
            }
            else
            {
                dir = static_cast<side_t>((dir + 1) % 4);
                ++counter;
            }

            if (counter > 3)
                break;
        }
    }
}

output_t day20(const input_t& input)
{
    auto tiles = parse_tiles(input);
    const auto& corners = get_corners(tiles);

    size_t prod = 1;
    for (auto* corner: corners)
    {
        prod *= corner->id;
    }

    orient_tiles(corners);

    tile_t* top_left_corner = get_left_top_corner(corners);
    print_tile_map_ids(top_left_corner);

    return { prod, 0 };
}

void day20_test()
{
    char text1[1099] =
        "Tile 2311:\n"
        "..##.#..#.\n##..#.....\n#...##..#.\n####.#...#\n##.##.###.\n"
        "##...#.###\n.#.#.#..##\n..#....#..\n###...#.#.\n..###..###\n"
        "\n"
        "Tile 1951:\n"
        "#.##...##.\n#.####...#\n.....#..##\n#...######\n.##.#....#\n"
        ".###.#####\n###.##.##.\n.###....#.\n..#.#..#.#\n#...##.#..\n"
        "\n"
        "Tile 1171:\n"
        "####...##.\n#..##.#..#\n##.#..#.#.\n.###.####.\n..###.####\n"
        ".##....##.\n.#...####.\n#.##.####.\n####..#...\n.....##...\n"
        "\n"
        "Tile 1427:\n"
        "###.##.#..\n.#..#.##..\n.#.##.#..#\n#.#.#.##.#\n....#...##\n"
        "...##..##.\n...#.#####\n.#.####.#.\n..#..###.#\n..##.#..#.\n"
        "\n"
        "Tile 1489:\n"
        "##.#.#....\n..##...#..\n.##..##...\n..#...#...\n#####...#.\n"
        "#..#.#.#.#\n...#.#.#..\n##.#...##.\n..##.##.##\n###.##.#..\n"
        "\n"
        "Tile 2473:\n"
        "#....####.\n#..#.##...\n#.##..#...\n######.#.#\n.#...#.#.#\n"
        ".#########\n.###.#..#.\n########.#\n##...##.#.\n..###.#.#.\n"
        "\n"
        "Tile 2971:\n"
        "..#.#....#\n#...###...\n#.#.###...\n##.##..#..\n.#####..##\n"
        ".#..####.#\n#..#.#..#.\n..####.###\n..#.#.###.\n...#.#.#.#\n"
        "\n"
        "Tile 2729:\n"
        "...#.#.#.#\n####.#....\n..#.#.....\n....#..#.#\n.##..##.#.\n"
        ".#.####...\n####.#.#..\n##.####...\n##..#.##..\n#.##...##.\n"
        "\n"
        "Tile 3079:\n"
        "#.#.#####.\n.#..######\n..#.......\n######....\n####.#..#.\n"
        ".#...#.##.\n#.#####.##\n..#.###...\n..#.......\n..#.###...\n"
        "\n";
    input_t test1 { text1, sizeof(text1) };

    auto tiles = parse_tiles(test1);
    const auto& corners = get_corners(tiles);

    // Part 1
    {
        size_t prod = 1;
        for (auto* corner: corners)
        {
            prod *= corner->id;
        }
        assert(prod == 20899048083289);
    }

    // Part 2
    {
        orient_tiles(corners);

        tile_t* top_left_corner = get_left_top_corner(corners);
        print_tile_map_ids(top_left_corner);
    }
}
