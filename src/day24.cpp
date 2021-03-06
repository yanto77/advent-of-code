#include "advent2020.h"

namespace
{
    constexpr size_t MAP_DIM = 100;
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
        for (const auto& column: map)
            for (bool cell: column)
                count += static_cast<int>(cell);
        return count;
    }
}

output_t day24(const input_t& input)
{
    const auto& map = parse_and_fill(input);
    size_t part1 = count_flipped(map);
    return { part1, 0 };
}

void day24_test()
{
    char text1[] =
        "sesenwnenenewseeswwswswwnenewsewsw\n"
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

    const auto& map = parse_and_fill(test1);
    assert(10 == count_flipped(map));
}
