#include "advent2020.h"

namespace
{
    enum side_t { TOP, RIGHT, BOTTOM, LEFT };

    struct tile_t
    {
        // matrix of 10 x 10 entries
        size_t id = 0;
        std::array<std::array<bool, 10>, 4> sides = {};
        std::array<std::array<bool, 8>, 8> inner = {};
        std::array<tile_t*, 4> side_tiles = {};

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
            printf("ID: %zu\n", id);

            print_side(TOP);
            print_side(RIGHT);
            print_side(BOTTOM);
            print_side(LEFT);

            printf("Inner:\n");
            for (int y = 0; y < inner.size(); ++y)
            {
                for (int x = 0; x < inner[0].size(); ++x)
                {
                    printf("%c", inner[y][x] ? '#' : '.');
                }
                printf("\n");
            }
            printf("\n");
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

    bool match_two_sides(const std::array<bool, 10>& side1, const std::array<bool, 10>& side2)
    {
        std::array<bool, 10> side1r;
        std::reverse_copy(std::begin(side1), std::end(side1), std::begin(side1r));
        return side1 == side2 || side1r == side2;
    }

    void check_two_tiles(std::vector<tile_t>& tiles, size_t t1, size_t t2)
    {
        constexpr std::array<side_t, 4> sides = { TOP, RIGHT, BOTTOM, LEFT };
        for (const side_t s1: sides)
        {
            if (tiles[t1].side_tiles[s1] != nullptr)
                continue;

            for (const side_t s2: sides)
            {
                if (tiles[t2].side_tiles[s2] != nullptr)
                    continue;

                if (match_two_sides(tiles[t1].sides[s1], tiles[t2].sides[s2]))
                {
                    // printf("comparing tiles: %zu + %zu, sides: %d + %d\n", t1, t2, s1, s2);
                    // printf(" - "); tiles[t1].print_side(s1);
                    // printf(" - "); tiles[t2].print_side(s2);
                    // printf(COLOR_GREEN() " - found match!\n" COLOR_RESET());

                    tiles[t1].side_tiles[s1] = &tiles[t2];
                    tiles[t2].side_tiles[s2] = &tiles[t1];
                    return;
                }
            }
        }
    }

    void combine_tiles(std::vector<tile_t>& tiles)
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
    }

    std::array<const tile_t*, 4> get_corners(const std::vector<tile_t>& tiles)
    {
        std::array<const tile_t*, 4> corners {};
        size_t idx = 0;

        for (size_t i = 0; i < tiles.size(); ++i)
        {
            size_t count = 0;
            count += static_cast<int>(tiles[i].side_tiles[TOP] != nullptr);
            count += static_cast<int>(tiles[i].side_tiles[RIGHT] != nullptr);
            count += static_cast<int>(tiles[i].side_tiles[BOTTOM] != nullptr);
            count += static_cast<int>(tiles[i].side_tiles[LEFT] != nullptr);

            // printf("tile: %zu\n", tiles[i].id);
            // printf("  - %zu \n", tiles[i].side_tiles[TOP] ? tiles[i].side_tiles[TOP]->id : 0);
            // printf("  - %zu \n", tiles[i].side_tiles[RIGHT] ? tiles[i].side_tiles[RIGHT]->id : 0);
            // printf("  - %zu \n", tiles[i].side_tiles[BOTTOM] ? tiles[i].side_tiles[BOTTOM]->id : 0);
            // printf("  - %zu \n", tiles[i].side_tiles[LEFT] ? tiles[i].side_tiles[LEFT]->id : 0);
            // printf("  - count: %zu\n", count);

            if (count == 2)
            {
                corners[idx++] = &tiles[i];
            }
        }

        return corners;
    }

    [[maybe_unused]]
    void print_id_map(const std::vector<tile_t>& tiles)
    {
        const tile_t* left_top_corner = nullptr;
        for (size_t i = 0; i < tiles.size(); ++i)
        {
            printf("tile: %zu\n", tiles[i].id);
            printf("  - %zu \n", tiles[i].side_tiles[TOP] ? tiles[i].side_tiles[TOP]->id : 0);
            printf("  - %zu \n", tiles[i].side_tiles[RIGHT] ? tiles[i].side_tiles[RIGHT]->id : 0);
            printf("  - %zu \n", tiles[i].side_tiles[BOTTOM] ? tiles[i].side_tiles[BOTTOM]->id : 0);
            printf("  - %zu \n", tiles[i].side_tiles[LEFT] ? tiles[i].side_tiles[LEFT]->id : 0);

            if (tiles[i].side_tiles[TOP] == nullptr &&
                tiles[i].side_tiles[RIGHT] != nullptr &&
                tiles[i].side_tiles[BOTTOM] != nullptr &&
                tiles[i].side_tiles[LEFT] == nullptr)
            {
                left_top_corner = &tiles[i];
                printf(COLOR_GREEN() "  - left top!\n" COLOR_RESET());
                break;
            }
        }

        // int i = 0;

        // const tile_t* curr_row = left_top_corner;
        // while (curr_row->side_tiles[BOTTOM] != nullptr)
        // {
        //     const tile_t* curr_col = curr_row;
        //     while (curr_col->side_tiles[RIGHT] != nullptr)
        //     {
        //         printf("[ %zu ]", curr_col->id);
        //         curr_col = curr_col->side_tiles[RIGHT];

        //         if (i > 10)
        //             return;
        //         ++i;
        //     }
        //     printf("\n");

        //     curr_row = curr_row->side_tiles[BOTTOM];
        // }
    }
}

output_t day20(const input_t& input)
{
    auto tiles = parse_tiles(input);
    combine_tiles(tiles);

    size_t prod = 1;
    for (auto corner: get_corners(tiles))
    {
        prod *= corner->id;
    }

    // assert(prod == 20899048083289);
    return { prod, 0 };
}

void day20_test()
{
    char text1[1099] =
        "Tile 2311:\n"
        "..##.#..#.\n"
        "##..#.....\n"
        "#...##..#.\n"
        "####.#...#\n"
        "##.##.###.\n"
        "##...#.###\n"
        ".#.#.#..##\n"
        "..#....#..\n"
        "###...#.#.\n"
        "..###..###\n"
        "\n"
        "Tile 1951:\n"
        "#.##...##.\n"
        "#.####...#\n"
        ".....#..##\n"
        "#...######\n"
        ".##.#....#\n"
        ".###.#####\n"
        "###.##.##.\n"
        ".###....#.\n"
        "..#.#..#.#\n"
        "#...##.#..\n"
        "\n"
        "Tile 1171:\n"
        "####...##.\n"
        "#..##.#..#\n"
        "##.#..#.#.\n"
        ".###.####.\n"
        "..###.####\n"
        ".##....##.\n"
        ".#...####.\n"
        "#.##.####.\n"
        "####..#...\n"
        ".....##...\n"
        "\n"
        "Tile 1427:\n"
        "###.##.#..\n"
        ".#..#.##..\n"
        ".#.##.#..#\n"
        "#.#.#.##.#\n"
        "....#...##\n"
        "...##..##.\n"
        "...#.#####\n"
        ".#.####.#.\n"
        "..#..###.#\n"
        "..##.#..#.\n"
        "\n"
        "Tile 1489:\n"
        "##.#.#....\n"
        "..##...#..\n"
        ".##..##...\n"
        "..#...#...\n"
        "#####...#.\n"
        "#..#.#.#.#\n"
        "...#.#.#..\n"
        "##.#...##.\n"
        "..##.##.##\n"
        "###.##.#..\n"
        "\n"
        "Tile 2473:\n"
        "#....####.\n"
        "#..#.##...\n"
        "#.##..#...\n"
        "######.#.#\n"
        ".#...#.#.#\n"
        ".#########\n"
        ".###.#..#.\n"
        "########.#\n"
        "##...##.#.\n"
        "..###.#.#.\n"
        "\n"
        "Tile 2971:\n"
        "..#.#....#\n"
        "#...###...\n"
        "#.#.###...\n"
        "##.##..#..\n"
        ".#####..##\n"
        ".#..####.#\n"
        "#..#.#..#.\n"
        "..####.###\n"
        "..#.#.###.\n"
        "...#.#.#.#\n"
        "\n"
        "Tile 2729:\n"
        "...#.#.#.#\n"
        "####.#....\n"
        "..#.#.....\n"
        "....#..#.#\n"
        ".##..##.#.\n"
        ".#.####...\n"
        "####.#.#..\n"
        "##.####...\n"
        "##..#.##..\n"
        "#.##...##.\n"
        "\n"
        "Tile 3079:\n"
        "#.#.#####.\n"
        ".#..######\n"
        "..#.......\n"
        "######....\n"
        "####.#..#.\n"
        ".#...#.##.\n"
        "#.#####.##\n"
        "..#.###...\n"
        "..#.......\n"
        "..#.###...\n"
        "\n";
    input_t test1 { text1, sizeof(text1) };

    auto tiles = parse_tiles(test1);
    combine_tiles(tiles);
    // print_id_map(tiles);

    size_t prod = 1;
    for (auto corner: get_corners(tiles))
    {
        prod *= corner->id;
    }

    assert(prod == 20899048083289);
}
