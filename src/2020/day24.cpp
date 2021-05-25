#include "advent2020.h"

ADVENT_DAY(2020, 24, 438, 4038);

namespace
{
    constexpr size_t MAP_DIM = 150;
    typedef std::array<std::array<bool, MAP_DIM>, MAP_DIM> hex_map_t;
    constexpr vec2i MAP_CENTER = { MAP_DIM / 2, MAP_DIM / 2 };

    hex_map_t parse_and_fill(const input_t& input)
    {
        hex_map_t hex_map {};

        parse_input(input, [&](const sv& line)
        {
            // std::cout << line << std::endl;

            vec2i tile = MAP_CENTER;
            for (size_t idx = 0; idx < line.size(); ++idx)
            {
                switch (line[idx])
                {
                    case 'e': tile += vec2i{1, 0}; break;
                    case 'w': tile += vec2i{-1, 0}; break;
                    case 'n': tile += (line[++idx] == 'w' ? vec2i{0, -1} : vec2i{1, -1}); break;
                    case 's': tile += (line[++idx] == 'w' ? vec2i{-1, 1} : vec2i{0, 1}); break;
                    default: break;
                }
            }

            // printf("-> tile (%d, %d)\n", tile.x, tile.y);
            hex_map[tile.y][tile.x] = !hex_map[tile.y][tile.x];
        });

        return hex_map;
    }

    size_t count_flipped(const hex_map_t& map)
    {
        size_t count = 0;
        for (const auto& column : map)
            for (bool cell : column)
                count += static_cast<int>(cell);
        return count;
    }

    hex_map_t simulate(hex_map_t map, size_t n_days)
    {
        for (size_t day = 0; day < n_days; ++day)
        {
            hex_map_t copy = map;
            for (size_t y = 1; y < (MAP_DIM - 1); ++y)
            {
                for (size_t x = 1; x < (MAP_DIM - 1); ++x)
                {
                    constexpr static std::array<vec2i, 6> directions = {
                        vec2i{1, 0}, vec2i{0, 1}, vec2i{-1, 1},
                        vec2i{-1, 0}, vec2i{0, -1}, vec2i{1, -1}
                    };

                    size_t neighbors = 0;
                    for (const auto& dir : directions)
                    {
                        neighbors += static_cast<int>(map[y + dir.y][x + dir.x]);
                    }

                    constexpr static bool white = false;
                    constexpr static bool black = true;
                    if (map[y][x] == black && (neighbors == 0 || neighbors > 2))
                    {
                        copy[y][x] = white;
                    }
                    else if (map[y][x] == white && neighbors == 2)
                    {
                        copy[y][x] = black;
                    }
                }
            }

            // printf("Day %zu: %zu\n", day+1, count_flipped(copy));
            map = copy;
        }

        return map;
    }
}

output_t Day_2020_24::run_solution(const input_t& input) const
{
    const auto& map = parse_and_fill(input);
    size_t part1 = count_flipped(map);
    size_t part2 = count_flipped(simulate(map, 100));
    return { part1, part2 };
}

void Day_2020_24::run_tests() const
{
    char text1[641] = "sesenwnenenewseeswwswswwnenewsewsw\n"
                      "neeenesenwnwwswnenewnwwsewnenwseswesw\n"
                      "seswneswswsenwwnwse\n"
                      "nwnwneseeswswnenewneswwnewseswneseene\n"
                      "swweswneswnenwsewnwneneseenw\n"
                      "eesenwseswswnenwswnwnwsewwnwsene\n"
                      "sewnenenenesenwsewnenwwwse\n"
                      "wenwwweseeeweswwwnwwe\n"
                      "wsweesenenewnwwnwsenewsenwwsesesenwne\n"
                      "neeswseenwwswnwswswnw\n"
                      "nenwswwsewswnenenewsenwsenwnesesenew\n"
                      "enewnwewneswsewnwswenweswnenwsenwsw\n"
                      "sweneswneswneneenwnewenewwneswswnese\n"
                      "swwesenesewenwneswnwwneseswwne\n"
                      "enesenwswwswneneswsenwnewswseenwsese\n"
                      "wnwnesenesenenwwnenwsewesewsesesew\n"
                      "nenewswnwewswnenesenwnesewesw\n"
                      "eneswnwswnwsenenwnwnwwseeswneewsenese\n"
                      "neswnwewnwnwseenwseesewsenwsweewe\n"
                      "wseweeenwnesenwwwswnew\n";
    input_t test1 { text1, sizeof(text1) };

    const auto& init_map = parse_and_fill(test1);
    assert(10 == count_flipped(init_map));
    assert(37 == count_flipped(simulate(init_map, 10)));
    assert(132 == count_flipped(simulate(init_map, 20)));
    assert(566 == count_flipped(simulate(init_map, 50)));
    assert(1106 == count_flipped(simulate(init_map, 70)));
    assert(2208 == count_flipped(simulate(init_map, 100)));
}
