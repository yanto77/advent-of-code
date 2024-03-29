#include "setup/all.h"

ADVENT_DAY(2021, 2, 2272262, 2134882034);

result_t Day_2021_2::run_solution(str_view input) const
{
    vec2i p1_pos {0, 0}; // { horizontal pos, depth }
    vec3i p2_pos {0, 0, 0}; // { horizontal pos, depth, aim }

    for_each_split(input, '\n', [&](str_view line)
    {
        switch (line[0])
        {
            case 'f': // forward
            {
                auto value = parse_uint(line[sizeof("forward")]);
                p1_pos.x += value;
                p2_pos += { value, p2_pos.z * value, 0};
                break;
            }
            case 'd': // down
            {
                auto value = parse_uint(line[sizeof("down")]);
                p1_pos.y += value;
                p2_pos.z += value;
                break;
            }
            case 'u': // up
            {
                auto value = parse_uint(line[sizeof("up")]);
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
