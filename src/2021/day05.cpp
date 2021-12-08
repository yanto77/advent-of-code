#include "setup/all.h"

ADVENT_DAY(2021, 5, 6687, 19851);

namespace
{
    struct line_t { vec2i p1; vec2i p2; };

    // considers point on a segment (non-infinite line)
    // ref: https://stackoverflow.com/a/11908158
    bool is_point_on_segment(const vec2i& point, const line_t& line)
    {
        const int dxc = point.x - line.p1.x;
        const int dyc = point.y - line.p1.y;

        const int dxl = line.p2.x - line.p1.x;
        const int dyl = line.p2.y - line.p1.y;

        const int cross = dxc * dyl - dyc * dxl;

        if (cross != 0)
            return false;

        if (abs(dxl) >= abs(dyl))
            return dxl > 0 ? 
                line.p1.x <= point.x && point.x <= line.p2.x :
                line.p2.x <= point.x && point.x <= line.p1.x;
        else
            return dyl > 0 ? 
                line.p1.y <= point.y && point.y <= line.p2.y :
                line.p2.y <= point.y && point.y <= line.p1.y;
    }

    std::vector<line_t> parse_lines(const input_t& input)
    {
        std::vector<line_t> lines;
    
        parse_input(input, [&](const sv& line)
        {
            const auto& [p1_str, p2_str] = split_single(line, " -> ");

            const auto& [p1x, p1y] = split_single(p1_str, ",");
            const auto& [p2x, p2y] = split_single(p2_str, ",");

            vec2i p1 { to_int<uint16_t>(p1x), to_int<uint16_t>(p1y) };
            vec2i p2 { to_int<uint16_t>(p2x), to_int<uint16_t>(p2y) };

            // fmt::print("line: {}\n", line);
            // fmt::print(" -> p1: ({}, {}), p2: ({}, {})\n", p1.x, p1.y, p2.x, p2.y);

            // assert(p1.x <= 1000);
            // assert(p1.y <= 1000);
            // assert(p2.x <= 1000);
            // assert(p2.y <= 1000);

            lines.push_back(line_t { p1, p2 });
        });

        return lines;
    }

    size_t compute_overlapping_lines(const std::vector<line_t>& lines, bool ignore_diagonals)
    {
        size_t counter = 0;

        for (int x = 0; x < 1000; x++)
        {
            for (int y = 0; y < 1000; y++)
            {
                size_t cell_counter = 0;
                vec2i point {y, x};

                for (const auto& line: lines)
                {
                    if (ignore_diagonals && line.p1.x != line.p2.x && line.p1.y != line.p2.y)
                        continue;

                    if (is_point_on_segment(point, line))
                        cell_counter++;
                }

                counter += (cell_counter >= 2);
            }
        }

        return counter;
    }
}

output_t Day_2021_5::run_solution(const input_t& input) const
{
    std::vector<line_t> lines = parse_lines(input);

    size_t part1 = compute_overlapping_lines(lines, true);
    size_t part2 = compute_overlapping_lines(lines, false);

    return { part1, part2 };
}

void Day_2021_5::run_tests() const
{
    char text1[] = 
        "0,9 -> 5,9\n"
        "8,0 -> 0,8\n"
        "9,4 -> 3,4\n"
        "2,2 -> 2,1\n"
        "7,0 -> 7,4\n"
        "6,4 -> 2,0\n"
        "0,9 -> 2,9\n"
        "3,4 -> 1,4\n"
        "0,0 -> 8,8\n"
        "5,5 -> 8,2\n";
    input_t test1 { text1, sizeof(text1) };

    std::vector<line_t> lines = parse_lines(test1);
    size_t part1 = compute_overlapping_lines(lines, true);
    assert(5 == part1);
    size_t part2 = compute_overlapping_lines(lines, false);
    assert(12 == part2);
}
