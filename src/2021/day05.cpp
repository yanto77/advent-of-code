#include "setup/all.h"

ADVENT_DAY(2021, 5, 6687, 19851);

namespace
{
    struct line_t { vec2i p1; vec2i p2; };

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

            lines.push_back(line_t { p1, p2 });
        });

        return lines;
    }

    size_t compute_overlapping_lines(const std::vector<line_t>& lines, bool ignore_diagonals)
    {
        std::array<std::array<uint8_t, 1000>, 1000> map {};

        // paint lines on the map
        for (const auto& line: lines)
        {
            if (line.p1.x == line.p2.x)
            {
                int min_row = min(line.p1.y, line.p2.y);
                int max_row = max(line.p1.y, line.p2.y);
                for (int row = min_row; row < max_row + 1; row++)
                    map[row][line.p1.x] += 1;
            }
            else if (line.p1.y == line.p2.y)
            {
                int min_col = min(line.p1.x, line.p2.x);
                int max_col = max(line.p1.x, line.p2.x);
                for (int col = min_col; col < max_col + 1; col++)
                    map[line.p1.y][col] += 1;
            }
            else if (!ignore_diagonals)
            {
                vec2i pos = line.p1;
                vec2i delta = {
                    (line.p1.x < line.p2.x) ? 1 : -1,
                    (line.p1.y < line.p2.y) ? 1 : -1,
                };

                map[pos.y][pos.x] += 1;

                while(true)
                {
                    pos += delta;
                    map[pos.y][pos.x] += 1;
                    if (pos == line.p2)
                        break;
                }
            }
        }

        size_t counter = 0;
        for (const auto& row: map)
            for (uint8_t cell: row)
                counter += (cell >= 2);
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
