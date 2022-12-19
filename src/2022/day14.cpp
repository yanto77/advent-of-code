#include "setup/all.h"
#include <stack>
#include <optional>
#include <thread>
#include <chrono>

ADVENT_DAY(2022, 14, 828, 25500);

namespace 
{
    constexpr size_t XLIM = 1000;
    constexpr size_t YLIM = 200;

    struct map_t
    {
        std::array<std::array<bool, XLIM>, YLIM> data {};

        map_t(str_view input)
        {
            parse_map(input);
        }

        void parse_map(str_view input)
        {
            for_each_split(input, '\n', [&](str_view line)
            {
                std::optional<vec2i> p1;
                for_each_split(line, " -> ", [&](str_view part)
                {
                    const auto& [xstr, ystr] = split_at(part, ',');
                    vec2i p2 { to_int<int32_t>(xstr), to_int<int32_t>(ystr) };
                    if (p1.has_value())
                    {
                        draw_line(p1.value(), p2);
                    }
                    p1 = p2;
                });
            });
        }

        size_t count_nodes() const
        {
            size_t count = 0;

            for (int32_t y = 0; y < data.size(); y++)
                for (int32_t x = 0; x < data[0].size(); x++)
                    count += data[y][x];

            return count;
        }

        size_t simulate_sand()
        {
            const size_t n_blocks = count_nodes();
            const vec2i start = { 500, 0 };

            vec2i point = start;
            while (true) // for each sand
            {
                if (data[start.y][start.x] ||
                    !in_range(point.x, 1, (int32_t)XLIM - 1) || 
                    !in_range(point.y, 0, (int32_t)YLIM - 1))
                {
                    break;
                }

                if (!data[point.y + 1][point.x]) // bottom
                {
                    point.y += 1;
                }
                else if (!data[point.y + 1][point.x - 1]) // left
                {
                    point.x -= 1;
                    point.y += 1;
                }
                else if (!data[point.y + 1][point.x + 1]) // right
                {
                    point.x += 1;
                    point.y += 1;
                }
                else // can't move
                {
                    data[point.y][point.x] = true;
                    point = start;
                }
            }

            size_t n_extra_blocks = count_nodes() - n_blocks;
            return n_extra_blocks;
        }

        [[maybe_unused]]
        void print_nodes() const
        {
            auto [ xmin, xmax, ymin, ymax ] = get_min_max();
            fmt::print("min/max: {}, {}, {}, {}\n", xmin, xmax, ymin, ymax);
            for (int32_t x = xmin; x < xmax; x++)
            {
                for (int32_t y = 0; y < ymax; y++)
                {
                    if (y == 0 && x == 500) fmt::print("+");
                    else if (data[y][x]) fmt::print("#");
                    else fmt::print(".");
                }
                fmt::print("\n");
            }
        }

        void draw_line(vec2i start, vec2i end)
        {
            vec2i point = start;
            vec2i delta = clamp(end - point, -1, 1);
            while (point != end)
            {
                data[point.y][point.x] = true;
                point += delta;
            }
            data[point.y][point.x] = true;
        }

        std::array<int32_t, 4> get_min_max() const
        {
            int32_t xmin = INT32_MAX;
            int32_t xmax = INT32_MIN;
            int32_t ymin = INT32_MAX;
            int32_t ymax = INT32_MIN;
            for (int32_t y = 0; y < data.size(); y++)
            {
                for (int32_t x = 0; x < data[0].size(); x++)
                {
                    if (data[y][x])
                    {
                        xmin = min(xmin, x);
                        xmax = max(xmax, x);
                        ymin = min(ymin, y);
                        ymax = max(ymax, y);
                    }
                }
            }
            return { xmin, xmax, ymin, ymax };
        }
    };
}

result_t Day_2022_14::run_solution(str_view input) const
{
    map_t map(input);
    size_t part1 = map.simulate_sand();

    auto [ xmin, xmax, ymin, ymax ] = map.get_min_max();
    map.draw_line(vec2i {0, ymax+2}, vec2i {XLIM, ymax+2});

    size_t part2 = map.simulate_sand() + part1;
    // map.print_nodes();

    return { part1, part2 };
}

void Day_2022_14::run_tests() const
{
}
