#include "setup/all.h"

ADVENT_DAY(2022, 12, 449, 443);

namespace 
{
    // Refs:
    // - https://www.redblobgames.com/pathfinding/a-star/introduction.html
    template <size_t X_MAX, size_t Y_MAX>
    struct map_t
    {
        using point_t = vec2<int32_t>;
        static constexpr point_t DIRS[4] = {{-1, 0}, {0, +1}, {+1, 0}, {0, -1}};
        std::array<std::array<int8_t, X_MAX>, Y_MAX> data {}; // height data
        std::array<std::array<bool, X_MAX>, Y_MAX> visited {};
        
        point_t p_start {};
        point_t p_end {};

        map_t(str_view input)
        {
            for (int32_t y = 0; y < Y_MAX; y++)
            {
                for (int32_t x = 0; x < X_MAX; x++)
                {
                    char cell = input[y*(X_MAX+1) + x];
                    if (cell == 'S')
                    {
                        p_start = { x, y };
                        cell = 'a';
                    }
                    else if (cell == 'E')
                    {
                        p_end = { x, y };
                        cell = 'z';
                    }
                    data[y][x] = cell - 'a';
                }
            }
        }

        void print_visited(point_t current)
        {
            // Ref: https://r-charts.com/color-palette-generator/
            // Ref: https://www.webfx.com/web-design/hex-to-rgb/
            [[maybe_unused]]
            static const char* CELL_COLORS[26] = {
                TRUE_COLOR(4,0,255),   TRUE_COLOR(66,0,245),  TRUE_COLOR(91,0,235),
                TRUE_COLOR(109,0,225), TRUE_COLOR(124,0,215), TRUE_COLOR(135,0,205),
                TRUE_COLOR(146,0,196), TRUE_COLOR(155,0,186), TRUE_COLOR(163,0,177),
                TRUE_COLOR(170,0,167), TRUE_COLOR(177,0,158), TRUE_COLOR(183,0,148),
                TRUE_COLOR(188,0,139), TRUE_COLOR(193,0,130), TRUE_COLOR(198,0,120),
                TRUE_COLOR(202,0,111), TRUE_COLOR(206,0,102), TRUE_COLOR(210,0,93),
                TRUE_COLOR(214,0,84),  TRUE_COLOR(217,0,75),  TRUE_COLOR(220,0,65),
                TRUE_COLOR(223,0,55),  TRUE_COLOR(226,0,45),  TRUE_COLOR(229,0,34),
                TRUE_COLOR(232,0,21),  TRUE_COLOR(234,4,0),
            };

            clear_screen();

            for (int32_t y = 0; y < Y_MAX; y++)
            {
                for (int32_t x = 0; x < X_MAX; x++)
                {
                    const point_t point = { x, y };

                    if (point == p_start)               fmt::print("{}S", COLOR_RED());
                    else if (point == p_end)            fmt::print("{}E", COLOR_GREEN());
                    else if (point == current)          fmt::print("{}#", COLOR_YELLOW());
                    else if (visited[point.y][point.x]) fmt::print("{}O", CELL_COLORS[data[y][x]]);
                    else                                fmt::print("{}.", CELL_COLORS[data[y][x]]); // heigh-based color
                }
                fmt::print("\n");
            }
            fmt::print(COLOR_RESET());
        }

        size_t bfs_part1()
        {
            std::deque<std::pair<point_t, size_t>> frontier = {{ p_start, 0 }}; // { point, distance }
            visited = {};

            while (!frontier.empty())
            {
                auto& [curr, distance] = frontier.front();
                frontier.pop_front();
                // print_visited(curr, visited);
                
                if (curr == p_end)
                {
                    return distance;
                }

                for (const point_t& dir: DIRS)
                {
                    const point_t next = curr + dir; // neighbor
                    if (!(0 <= next.y && next.y < Y_MAX) ||
                        !(0 <= next.x && next.x < X_MAX) ||
                        (data[next.y][next.x] - data[curr.y][curr.x]) > 1) // cannot jump this high
                    {
                        continue;
                    }

                    if (!visited[next.y][next.x])
                    {
                        frontier.push_back({next, distance+1});
                        visited[next.y][next.x] = true;
                    }
                }
            }

            return 0;
        }

        size_t bfs_part2()
        {
            std::deque<std::pair<point_t, size_t>> frontier = {{ p_end, 0 }}; // { point, distance }
            visited = {};

            while (!frontier.empty())
            {
                auto& [curr, distance] = frontier.front();
                frontier.pop_front();
                
                // print_visited(curr, visited);
                
                if (data[curr.y][curr.x] == 0) // first 'a'
                {
                    return distance;
                }

                for (uint8_t dirIdx = 0; dirIdx < 4; dirIdx++)
                {
                    const point_t next = curr + DIRS[dirIdx]; // neighbor
                    if (!(0 <= next.y && next.y < Y_MAX) ||
                        !(0 <= next.x && next.x < X_MAX) ||
                        (data[next.y][next.x] - data[curr.y][curr.x]) < -1) // cannot jump this high (in reverse)
                    {
                        continue;
                    }

                    if (!visited[next.y][next.x])
                    {
                        frontier.push_back({next, distance+1});
                        visited[next.y][next.x] = true;
                    }
                }
            }

            return 0;
        }
    };
}

result_t Day_2022_12::run_solution(str_view input) const
{
    map_t<132, 41> map(input);
    size_t part1 = map.bfs_part1();
    size_t part2 = map.bfs_part2();
    return { part1, part2 };
}

void Day_2022_12::run_tests() const
{
    std::string test_input = 
        "Sabqponm\n"
        "abcryxxl\n"
        "accszExk\n"
        "acctuvwj\n"
        "abdefghi\n";

    map_t<8, 5> map(test_input);
    assert(map.bfs_part1() == 31);
    assert(map.bfs_part2() == 29);
}
