#include "advent2020.h"

namespace
{
    enum side { TOP, RIGHT, BOTTOM, LEFT };

    struct tile_t
    {
        // matrix of 10 x 10 entries
        size_t id = 0;
        std::array<std::array<bool, 10>, 4> sides = {};
        std::array<std::array<bool, 8>, 8> inner = {};

        [[maybe_unused]]
        void print()
        {
            printf("ID: %zu\n", id);

            printf("Top:    ");
            for (int x = 0; x < sides[TOP].size(); ++x)
                printf("%c", sides[TOP][x] ? '#' : '.');
            printf("\n");

            printf("Right:  ");
            for (int x = 0; x < sides[RIGHT].size(); ++x)
                printf("%c", sides[RIGHT][x] ? '#' : '.');
            printf("\n");

            printf("Bottom: ");
            for (int x = 0; x < sides[BOTTOM].size(); ++x)
                printf("%c", sides[BOTTOM][x] ? '#' : '.');
            printf("\n");

            printf("Left:   ");
            for (int x = 0; x < sides[LEFT].size(); ++x)
                printf("%c", sides[LEFT][x] ? '#' : '.');
            printf("\n");

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
                // std::cout << row << ": " << line << std::endl;
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
}

output_t day20(const input_t& input)
{
    return { 0, 0 };
}

void day20_test()
{
    char text1[245] =
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
        "\n";
    input_t test1 { text1, sizeof(text1) };

    auto tiles = parse_tiles(test1);
    for (auto tile: tiles)
    {
        tile.print();
    }
}
