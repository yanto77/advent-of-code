#include "setup/all.h"
#include "intcode.h"
#include "helpers/vec2.h"

ADVENT_DAY(2019, 11, 1876, 1);

namespace
{
    constexpr size_t X = 150;
    constexpr size_t Y = 150;
    typedef std::array<std::array<bool, X>, Y> map_t;

    struct ship_t
    {
        vec2i pos;
        uint8_t dir; // clockwise, 0: up, 1: right, 2: down, 3: left
    };

    [[maybe_unused]]
    void print_color_map(const map_t& map)
    {
        for (const auto& row: map)
        {
            for (bool tile: row)
            {
                if (tile)
                    printf(COLOR_WHITE() "X" COLOR_RESET());
                else
                    printf(COLOR_BLACK() "." COLOR_RESET());
            }
            printf("\n");
        }
    }

    std::pair<map_t, map_t> paint_map(intcode_solver_t& solver, bool initial_color)
    {
        ship_t ship = { .pos = { Y / 2, X / 2}, .dir = 3};
        map_t color_map = {};
        map_t painted_map = {};

        color_map[ship.pos.y][ship.pos.x] = initial_color;

        while (!solver.is_halted)
        {
            const bool tile = color_map[ship.pos.y][ship.pos.x];

            // we always expect two outputs
            solver.execute(true, { static_cast<uint8_t>(tile) });
            solver.execute(true);

            const auto& output = solver.output_data;
            if (output.size() == 0) // robot is finished
                break;

            // apply paint 
            color_map[ship.pos.y][ship.pos.x] = static_cast<uint8_t>(output[0]);
            painted_map[ship.pos.y][ship.pos.x] = true;

            // turn robot
            int64_t turn = output[1];
            if (turn == 0) ship.dir += -1;
            else if (turn == 1) ship.dir += +1;
            ship.dir %= 4;

            // move forward
            const std::array<vec2i, 4> turns = 
            {   // { dy, dx }
                vec2i{ 1, 0 }, // up
                vec2i{ 0, 1 }, // right
                vec2i{ -1, 0 }, // down
                vec2i{ 0, -1 }, // left
            };
            ship.pos += turns[ship.dir];

            solver.output_data.clear();
        }

        return { painted_map, color_map };
    }
}

output_t Day_2019_11::run_solution(str_view input) const
{
    intcode_solver_t solver(input);
    const auto& [painted_map_pt1, color_map_pt1] = paint_map(solver, false);

    solver.reset();
    const auto& [painted_map_pt2, color_map_pt2] = paint_map(solver, true);

    size_t part1 = 0;
    for (const auto& row: painted_map_pt1)
        for (bool tile: row)
            part1 += static_cast<uint8_t>(tile);

    // part2 = CGPJCGCL
    // validate with:
    // print_color_map(color_map_pt2);
    (void)color_map_pt2;

    return {part1, 1};
}

void Day_2019_11::run_tests() const
{
}
