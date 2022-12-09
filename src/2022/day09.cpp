#include "setup/all.h"
#include <unordered_map>
#include <thread>
#include <set>
#include <chrono>

ADVENT_DAY(2022, 9, 6464, 2604);

namespace 
{
    struct motion_t { int8_t dx; int8_t dy; uint8_t len; };
    using motions_t = std::vector<motion_t>;

    struct map_t
    {
        std::array<std::array<bool, 512>, 512> data {};

        [[maybe_unused]]
        bool is_inside(vec2i cell) const
        {
            return (0 <= cell.y && cell.y < data.size()) && 
                   (0 <= cell.x && cell.x < data[0].size());
        }

        void set_cell(vec2i cell)
        {
            data[cell.y][cell.x] = true;
        }

        bool get_cell(vec2i cell) const
        {
            return data[cell.y][cell.x];
        }

        size_t get_visited_cells()
        {
            size_t visited = 0;
            for (int y = 0; y < data.size(); y++)
            {
                for (int x = 0; x < data[0].size(); x++)
                {
                    visited += (int)get_cell({x, y});
                }
            }
            return visited;
        }

        template <size_t N>
        [[maybe_unused]]
        void print(const std::array<vec2i, N>& knots)
        {
            clear_screen();

            const int delta = 50;
            const vec2i tail = knots.back();
            for (int x = (tail.x - delta); x < (tail.x + delta); x++)
            {
                for (int y = (tail.y - delta); y < (tail.y + delta); y++)
                {
                    const bool cell = get_cell({x, y});

                    if (!is_inside({x, y}))
                    { 
                        fmt::print(COLOR_GRAY() " "); 
                        goto end;
                    }

                    for (size_t i = 0; i < knots.size(); i++)
                    {
                        if (vec2i{x, y} == knots[i])
                        {
                            fmt::print(COLOR_GREEN() "X");
                            goto end;
                        }
                    }

                    if (cell)
                        fmt::print(COLOR_WHITE() ".");
                    else
                        fmt::print(COLOR_GRAY() ".");

                    end:
                        fmt::print(COLOR_RESET());
                }
                fmt::print("\n");
            }

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10ms);
        }
    };

    motions_t parse_motions(str_view input)
    {
        motions_t motions {};

        for_each_split(input, '\n', [&](str_view line)
        {
            uint8_t len = to_int<uint8_t>(str_view { &line[2], line.size() - 2 });
            switch (line[0]) // direction
            {
                case 'L': { motions.push_back({-1,  0, len}); break; }
                case 'R': { motions.push_back({+1,  0, len}); break; }
                case 'D': { motions.push_back({ 0, -1, len}); break; }
                case 'U': { motions.push_back({ 0, +1, len}); break; }
            }
        });

        return motions;
    }

    template <size_t N>
    size_t get_positions(const motions_t& motions)
    {
        map_t map {};
        std::array<vec2i, N> knots {};

        // Move the rope to the center of the map
        for (auto& knot: knots) knot = { 256, 256 };

        for (const auto [dx, dy, len]: motions)
        {
            for (int i = 0; i < len; i++)
            {
                // Move head
                knots[0] += { dx, dy };
                
                // Move following knots one by one
                for (size_t i = 1; i < knots.size(); i++)
                {
                    vec2i delta = (knots[i-1] - knots[i]);
                    if (abs(delta.x) > 1 || abs(delta.y) > 1)
                    {
                        knots[i] += clamp(delta, -1, 1);
                    }
                }

                map.set_cell(knots[N-1]);
                // map.print(knots);
            }
        }

        return map.get_visited_cells();
    }
}

result_t Day_2022_9::run_solution(str_view input) const
{
    motions_t motions = parse_motions(input);
    size_t part1 = get_positions<2>(motions);
    size_t part2 = get_positions<10>(motions);
    return { part1, part2 };
}

void Day_2022_9::run_tests() const
{
}
