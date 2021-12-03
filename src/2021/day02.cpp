#include "setup/all.h"

ADVENT_DAY(2021, 2, 2272262, 2134882034);

output_t Day_2021_2::run_solution(const input_t& input) const
{
    vec2i p1_pos {0, 0}; // { horizontal pos, depth }
    vec3i p2_pos {0, 0, 0}; // { horizontal pos, depth, aim }

    parse_input(input, [&](const sv& line)
    {
        switch (line[0])
        {
            case 'f': // forward
            {
                auto value = to_int<int8_t>(line[sizeof("forward")]);
                p1_pos.x += value;
                p2_pos += { value, p2_pos.z * value, 0};
                break;
            }
            case 'd': // down
            {
                auto value = to_int<int8_t>(line[sizeof("down")]);
                p1_pos.y += value;
                p2_pos.z += value;
                break;
            }
            case 'u': // up
            {
                auto value = to_int<int8_t>(line[sizeof("up")]);
                p1_pos.y -= value;
                p2_pos.z -= value;
                break;
            }
            default: assert(false);
        }
    });

    size_t part1 = p1_pos.x * p1_pos.y;
    size_t part2 = p2_pos.x * p2_pos.y;
    return { part1, part2 };
}

void Day_2021_2::run_tests() const
{
}
