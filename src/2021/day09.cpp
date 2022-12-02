#include "setup/all.h"

ADVENT_DAY(2021, 9, 423, 1198704);

namespace
{
    constexpr size_t X_DIM = 100;
    constexpr size_t Y_DIM = 100;
    constexpr size_t PAD = 1;
    typedef std::array<std::array<uint8_t, X_DIM + 2 * PAD>, Y_DIM + 2 * PAD> map_t;

    map_t get_padded_map(str_view input)
    {
        map_t map;
        for (int y = 0; y < Y_DIM + 2 * PAD; y++)
            for (int x = 0; x < X_DIM + 2 * PAD; x++)
                map[y][x] = 9;

        size_t in_y = 0;
        parse_input(input, [&](str_view line_str)
        {
            for (size_t in_x = 0; in_x < X_DIM; in_x++)
                map[in_y+PAD][in_x+PAD] = to_int<uint8_t>(line_str[in_x]);
            in_y++;
        });

        return map;
    }

    [[maybe_unused]]
    void print_basin_map(const map_t& map, const map_t& regions)
    {
        const std::vector<std::string> colors = {
            COLOR_RED(), COLOR_GREEN(), COLOR_YELLOW(), COLOR_BLUE(),
            COLOR_MAGENTA(), COLOR_CYAN(), COLOR_WHITE(),
        };

        for (int y = PAD; y < Y_DIM + PAD; y++)
        {
            for (int x = PAD; x < X_DIM + PAD; x++)
            {
                if (map[y][x] == 9) // wall
                {
                    fmt::print(COLOR_BLACK() "{}" COLOR_RESET(), map[y][x]);
                }
                else if (regions[y][x] == UINT8_MAX) // unvisited
                {
                    fmt::print(COLOR_GRAY() "{}" COLOR_RESET(), map[y][x]);
                }
                else
                {
                    auto c = colors[regions[y][x] % colors.size()];
                    auto str = c + "{}" COLOR_RESET();
                    fmt::print(str, map[y][x]);
                }

            }
            fmt::print("\n");
        }
    }

    map_t flood_fill(const map_t& map, const std::vector<vec2i>& points)
    {
        map_t regions = map;

        for (int y = 0; y < Y_DIM + 2 * PAD; y++)
            for (int x = 0; x < X_DIM + 2 * PAD; x++)
                regions[y][x] = UINT8_MAX;

        for (size_t basin = 0; basin < points.size(); basin++)
        {
            std::queue<vec2i> pq;
            pq.push(points[basin]);

            while (!pq.empty())
            {
                vec2i p = pq.front();
                pq.pop();

                regions[p.y][p.x] = static_cast<uint8_t>(basin);

                auto check_and_add = [&](int x, int y)
                {
                    bool unvisited = (regions[y][x] == UINT8_MAX);
                    bool wall_tile = (map[y][x] == 9);
                    if (unvisited && !wall_tile)
                        pq.push({ x, y });
                };

                check_and_add(p.x, p.y-1);
                check_and_add(p.x, p.y+1);
                check_and_add(p.x-1, p.y);
                check_and_add(p.x+1, p.y);
            }
        }

        return regions;
    }

    std::pair<size_t, size_t> get_both_parts(map_t map)
    {
        size_t part1 = 0;
        std::vector<vec2i> lows;

        for (int y = PAD; y < Y_DIM + PAD; y++)
        {
            for (int x = PAD; x < X_DIM + PAD; x++)
            {
                auto p = map[y][x];
                if (p < map[y-1][x] && p < map[y+1][x] && p < map[y][x-1] && p < map[y][x+1])
                {
                    part1 += 1 + p;
                    lows.push_back({ x, y });
                }
            }
        }

        map_t regions = flood_fill(map, lows);

        // count region sizes
        std::vector<size_t> counters(lows.size(), 0);
        for (int y = PAD; y < Y_DIM + PAD; y++)
            for (int x = PAD; x < X_DIM + PAD; x++)
                counters[regions[y][x]]++;

        // find top 3
        std::sort(counters.begin(), counters.end());
        const auto& end_it = counters.rbegin();
        size_t part2 = end_it[0] * end_it[1] * end_it[2];

        return { part1, part2 };
    }
}

output_t Day_2021_9::run_solution(str_view input) const
{
    map_t map = get_padded_map(input);
    const auto& [part1, part2] = get_both_parts(map);
    return { part1, part2 };
}

void Day_2021_9::run_tests() const
{
    // char text1[] =
    //     "2199943210\n"
    //     "3987894921\n"
    //     "9856789892\n"
    //     "8767896789\n"
    //     "9899965678\n"
    //     "";
    // size_t part1 = get_part1(text1);
    // fmt::print("test1: {}\n", part1);
}
