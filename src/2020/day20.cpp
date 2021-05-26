#include "setup/all.h"

ADVENT_DAY(2020, 20, 107399567124539, 1555);

namespace
{
    enum side_t
    {
        TOP,
        RIGHT,
        BOTTOM,
        LEFT
    };
    constexpr std::array<side_t, 4> SIDES = { TOP, RIGHT, BOTTOM, LEFT };
    constexpr size_t OUTER_N = 10;
    constexpr size_t INNER_N = 8;

    struct tile_t
    {
        // matrix of 10 x 10 pixels
        size_t id = 0;
        std::array<std::array<bool, OUTER_N>, 4> sides = {};
        std::array<std::array<bool, INNER_N>, INNER_N> inner = {};
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
                tile.id = to_int<int>(sv { line.data() + 5, 4 });

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
                            tile.inner[row - 1][col - 1] = value;

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
        for (const side_t s1 : SIDES)
        {
            if (tiles[t1].side_tiles[s1] != nullptr)
                continue;

            for (const side_t s2 : SIDES)
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

        std::array<tile_t*, 4> corners = {};
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

    tile_t* get_left_top_corner(const std::array<tile_t*, 4>& corners)
    {
        for (auto corner : corners)
            if (corner->side_tiles[TOP] == nullptr && corner->side_tiles[RIGHT] != nullptr)
                return corner;

        return nullptr;
    }

    // tile_t* mirror(tile_t*)
    // {
    //     printf("test");
    //     std::array<std::array<bool, 8>, 8> out;
    // }
    // typedef std::function<void(tile_t*)> tile_fn_t;
    // typedef std::pair<std::array<int, 4>, tile_fn_t> orientation_t;

    struct orientation_t
    {
        std::array<int, 4> sides;
        std::function<void(tile_t*)> fn;
        std::function<std::vector<std::vector<uint8_t>>(std::vector<std::vector<uint8_t>>)> fn2;
    };

    void flip(tile_t* t)
    {
        std::array<std::array<bool, INNER_N>, INNER_N> out;
        for (size_t row = 0; row < INNER_N; row++)
        {
            for (size_t col = 0; col < INNER_N; col++)
            {
                out[row][INNER_N - 1 - col] = t->inner[row][col];
            }
        }
        t->inner = out;
    }

    void rot90(tile_t* t)
    {
        std::array<std::array<bool, INNER_N>, INNER_N> out;
        for (size_t r = 0; r < INNER_N; r++)
        {
            for (size_t c = 0; c < INNER_N; c++)
            {
                out[c][INNER_N - 1 - r] = t->inner[r][c];
            }
        }
        t->inner = out;
    }

    std::vector<std::vector<uint8_t>> flip(const std::vector<std::vector<uint8_t>>& in)
    {
        const size_t N = in.size();
        std::vector<std::vector<uint8_t>> copy(N, std::vector<uint8_t>(N, 0));

        for (size_t row = 0; row < N; row++)
        {
            for (size_t col = 0; col < N; col++)
            {
                copy[row][N - 1 - col] = in[row][col];
            }
        }
        return copy;
    }

    std::vector<std::vector<uint8_t>> rot90(const std::vector<std::vector<uint8_t>>& in)
    {
        const size_t N = in.size();
        std::vector<std::vector<uint8_t>> copy(N, std::vector<uint8_t>(N, 0));

        for (size_t row = 0; row < N; row++)
        {
            for (size_t col = 0; col < N; col++)
            {
                copy[col][N-1-row] = in[row][col];
            }
        }
        return copy;
    }

    const std::array<orientation_t, 8> ORIENTATIONS = {
        orientation_t{ // rot0
            .sides = { 0, 1, 2, 3 },
            .fn = [](tile_t* /* t */) {},
            .fn2 = [](auto t) { return t; }},
        orientation_t{ // rot90
            .sides = { 3, 0, 1, 2 },
            .fn = [](tile_t* t) { rot90(t); },
            .fn2 = [](auto t) { return rot90(t); }},
        orientation_t{ // rot180
            .sides = { 2, 3, 0, 1 },
            .fn = [](tile_t* t) { rot90(t); rot90(t); },
            .fn2 = [](auto t) { t = rot90(t); t = rot90(t); return t; }},
        orientation_t{ // rot270
            .sides = { 1, 2, 3, 0 },
            .fn = [](tile_t* t) { rot90(t); rot90(t); rot90(t); },
            .fn2 = [](auto t) { t = rot90(t); t = rot90(t); t = rot90(t); return t; }},
        orientation_t{ // flip0
            .sides = { 0, 3, 2, 1 },
            .fn = [](tile_t* t) { flip(t); },
            .fn2 = [](auto t) { return flip(t); }},
        orientation_t{ // flip90
            .sides = { 1, 0, 3, 2 },
            .fn = [](tile_t* t) { flip(t); rot90(t); },
            .fn2 = [](auto t) { t = flip(t); t = rot90(t); return t; }},
        orientation_t{ // flip180
            .sides = { 2, 1, 0, 3 },
            .fn = [](tile_t* t) { flip(t); rot90(t); rot90(t); },
            .fn2 = [](auto t) { t = flip(t); t = rot90(t); t = rot90(t); return t; }},
        orientation_t{ // flip270
            .sides = { 3, 2, 1, 0 },
            .fn = [](tile_t* t) { flip(t); rot90(t); rot90(t); rot90(t); },
            .fn2 = [](auto t) { t = flip(t); t = rot90(t); t = rot90(t); t = rot90(t); return t; }},
    };

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
                for (const auto& o : ORIENTATIONS)
                {
                    std::array<tile_t*, 4> sides;
                    for (size_t i = 0; i < 4; ++i)
                    {
                        sides[i] = next_tile->side_tiles[o.sides[i]];
                    }

                    const side_t op_dir = static_cast<side_t>((dir + 2) % 4); // opposite direction
                    const side_t hook_dir = static_cast<side_t>((dir + 3) % 4); // hook direction

                    const tile_t* hook1 = tile->side_tiles[hook_dir];
                    const tile_t* hook2 = hook1 ? hook1->side_tiles[dir] : nullptr;

                    if (tile == sides[op_dir] && hook2 == sides[hook_dir])
                    {
                        // printf("preparing %zu: ", next_tile->id);

                        next_tile->side_tiles = sides;
                        o.fn(next_tile);

                        match = true;
                        break;
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

    std::vector<std::vector<uint8_t>> combine_tiles(tile_t* top_left_corner)
    {
        size_t N = 0;
        {
            tile_t* row = top_left_corner;
            while (row != nullptr)
            {
                ++N;
                row = row->side_tiles[BOTTOM];
            }
        }
        N *= INNER_N; // tile count to cell count

        std::vector<std::vector<uint8_t>> map(N, std::vector<uint8_t>(N, 0));
        {
            tile_t* row = top_left_corner;
            size_t tile_row_idx = 0;
            while (row != nullptr)
            {
                for (size_t cell_row = 0; cell_row < INNER_N; ++cell_row)
                {
                    tile_t* col = row;
                    size_t tile_col_idx = 0;
                    while (col != nullptr)
                    {
                        for (size_t cell_col = 0; cell_col < INNER_N; ++cell_col)
                        {
                            size_t out_row = INNER_N * tile_row_idx + cell_row;
                            size_t out_col = INNER_N * tile_col_idx + cell_col;
                            map[out_row][out_col] = col->inner[cell_row][cell_col];
                        }

                        col = col->side_tiles[RIGHT];
                        ++tile_col_idx;
                    }
                }

                row = row->side_tiles[BOTTOM];
                ++tile_row_idx;
            }
        }

        return map;
    }
}

namespace
{
    [[maybe_unused]]
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

    [[maybe_unused]]
    void print_tile_map_contents(tile_t* top_left_corner)
    {
        tile_t* row = top_left_corner;
        while (row != nullptr)
        {
            for (size_t cell_row = 0; cell_row < 8; ++cell_row)
            {
                tile_t* col = row;
                while (col != nullptr)
                {
                    for (size_t cell_col = 0; cell_col < 8; ++cell_col)
                    {
                        if (col->inner[cell_row][cell_col])
                            printf(COLOR_GREEN() "#" COLOR_RESET());
                        else
                            printf(COLOR_GRAY() "." COLOR_RESET());
                    }

                    printf(" ");
                    col = col->side_tiles[RIGHT];
                }
                printf("\n");
            }

            printf("\n");
            row = row->side_tiles[BOTTOM];
        }
    }

    constexpr auto get_monster()
    {
        constexpr std::array<const char*, 3> input = {
            "                  # ",
            "#    ##    ##    ###",
            " #  #  #  #  #  #   "
        };

        std::array<std::array<bool, 20>, 3> monster {};
        for (size_t row = 0; row < input.size(); ++row)
            for (size_t col = 0; input[row][col] != '\0'; ++col)
                monster[row][col] = (input[row][col] == '#');

        return monster;
    }

    size_t eval_part2(tile_t* top_left_corner)
    {
        // print_tile_map_ids(top_left_corner);
        // print_tile_map_contents(top_left_corner);

        auto map = combine_tiles(top_left_corner);
        const auto& monster = get_monster();

        const size_t N = map.size();
        const size_t mNr = monster.size();
        const size_t mNc = monster[0].size();

        bool orientation_found = false;
        for (const auto& orientation : ORIENTATIONS)
        {
            // auto s = orientation.sides;
            // printf("checking orientation: %d %d %d %d\n", s[0], s[1], s[2], s[3]);
            auto copy = orientation.fn2(map);

            for (size_t map_r = 0; map_r < N - mNr; ++map_r)
            {
                for (size_t map_c = 0; map_c < N; ++map_c)
                {
                    bool match = true;
                    for (size_t mon_r = 0; mon_r < mNr && match; ++mon_r)
                    {
                        for (size_t mon_c = 0; mon_c < mNc && match; ++mon_c)
                        {
                            int in = monster[mon_r][mon_c] ? 1 : 0;
                            if (in == 1 && in != copy[map_r + mon_r][map_c + mon_c])
                            {
                                match = false;
                            }
                        }
                    }

                    if (match)
                    {
                        orientation_found = true;
                        for (size_t mon_r = 0; mon_r < mNr; ++mon_r)
                        {
                            for (size_t mon_c = 0; mon_c < mNc; ++mon_c)
                            {
                                if (monster[mon_r][mon_c])
                                {
                                    copy[map_r + mon_r][map_c + mon_c] = 2;
                                }
                            }
                        }
                    }
                }
            }

            if (orientation_found)
            {
                // printf(COLOR_GREEN() "found the orientation!\n" COLOR_RESET());
                map = copy;
                break;
            }
        }

        // printf("map:\n");
        size_t part2 = 0;
        for (const auto& row : map)
        {
            for (int col : row)
            {
                if (col == 2)
                {
                    // printf(COLOR_RED() "O" COLOR_RESET());
                }
                else if (col == 1)
                {
                    // printf(COLOR_GREEN() "#" COLOR_RESET());
                    ++part2;
                }
                else
                {
                    // printf(COLOR_GRAY() "." COLOR_RESET());
                }
            }
            // printf("\n");
        }

        return part2;
    }
}

output_t Day_2020_20::run_solution(const input_t& input) const
{
    auto tiles = parse_tiles(input);
    const auto& corners = get_corners(tiles);

    size_t prod = 1;
    for (auto* corner : corners)
    {
        prod *= corner->id;
    }

    orient_tiles(corners);

    tile_t* top_left_corner = get_left_top_corner(corners);
    size_t part2 = eval_part2(top_left_corner);

    return { prod, part2 };
}

void Day_2020_20::run_tests() const
{
    char text1[] = "Tile 2311:\n"
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
        for (auto* corner : corners)
        {
            prod *= corner->id;
        }
        assert(prod == 20899048083289);
    }

    // Part 2
    {
        orient_tiles(corners);

        tile_t* top_left_corner = get_left_top_corner(corners);
        size_t part_2 = eval_part2(top_left_corner);
        assert(part_2 == 273);
    }
}
