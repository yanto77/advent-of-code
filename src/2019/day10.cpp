#include "setup/all.h"
#include <unordered_set>

ADVENT_DAY(2019, 10, 286, 0);

namespace
{
    typedef std::vector<std::vector<bool>> map_t;

    map_t convert_map(const input_t& input)
    {
        map_t out;

        parse_input(input, [&](const sv& line_str)
        {
            std::vector<bool> line_bit(line_str.size(), 0);

            for (size_t i = 0; i < line_str.size(); ++i)
                line_bit[i] = (line_str[i] == '#');

            out.push_back(line_bit);
        });

        return out;
    }

    std::pair<vec2i, size_t> find_asteroid(const map_t& map)
    {
        const size_t y_dim = map.size();
        const size_t x_dim = map[0].size();

        std::unordered_map<vec2i, size_t> counts;

        for (int32_t y1 = 0; y1 < y_dim; ++y1)
        {
            for (int32_t x1 = 0; x1 < x_dim; ++x1)
            {
                if (!map[y1][x1])
                    continue;

                std::array<std::unordered_set<float>, 4> deltas;

                const vec2i asteroid1 { x1, y1 };
                for (int32_t y2 = 0; y2 < y_dim; ++y2)
                {
                    for (int32_t x2 = 0; x2 < x_dim; ++x2)
                    {
                        if (!map[y2][x2])
                            continue;

                        const vec2i asteroid2 { x2, y2 };
                        if (asteroid1 == asteroid2)
                            continue;

                        const vec2i delta = (asteroid1 - asteroid2);
                        float x = static_cast<float>(delta.y);
                        float y = static_cast<float>(delta.x);
                        float slope = y / x;

                        uint8_t bucket = std::signbit(x) * 2 + std::signbit(y);
                        deltas[bucket].insert(slope);

                        // size_t asd2 = 0;
                        // for (const auto& asd: deltas)
                        // {
                        //     asd2 += asd.size();
                        // }

                        // fmt::print("checking: ({}, {}) <-> ({}, {}), delta: ({}, {}), slope: {}, bucket: {}, count: {}\n", 
                        //     asteroid1.x, asteroid1.y, 
                        //     asteroid2.x, asteroid2.y, 
                        //     delta.x, delta.y, 
                        //     slope,
                        //     bucket,
                        //     asd2
                        // );
                    }
                }


                counts[asteroid1] = 0;
                for (const auto& bucket: deltas)
                {
                    counts[asteroid1] += bucket.size();
                }
                // printf("  => adding count: %zu\n\n", counts[asteroid1]);
            }
        }

        // for (auto& asd: counts)
        // {
        //     fmt::print("key: ({}, {}), count: {}\n", asd.first.x, asd.first.y, asd.second);
        // }
    
        vec2i max_asteroid;
        size_t max_count = 0;

        for (const auto& count: counts)
        {
            if (count.second > max_count)
            {
                max_count = count.second;
                max_asteroid = count.first;
            }
        }

        return { max_asteroid, max_count };
    }
}

output_t Day_2019_10::run_solution(const input_t& input) const
{
    map_t map = convert_map(input);
    const auto& [asteroid, part1] = find_asteroid(map);

    return {part1, 0};
}

void Day_2019_10::run_tests() const
{
    std::vector<std::string> inputs = 
    {
        std::string(".#..#\n"".....\n""#####\n""....#\n""...##\n"),
        std::string(
            "......#.#.\n""#..#.#....\n""..#######.\n"".#.#.###..\n"".#..#.....\n"
            "..#....#.#\n""#..#....#.\n"".##.#..###\n""##...#..#.\n"".#....####\n"
        ),
        std::string(
            "#.#...#.#.\n"".###....#.\n"".#....#...\n""##.#.#.#.#\n""....#.#.#.\n"
            ".##..###.#\n""..#...##..\n""..##....##\n""......#...\n"".####.###.\n"
        ),
        std::string(
            ".#..#..###\n""####.###.#\n""....###.#.\n""..###.##.#\n""##.##.#.#.\n"
            "....###..#\n""..#.#..#.#\n""#..#.#.###\n"".##...##.#\n"".....#.#..\n"
        ),
        std::string( 
            ".#..##.###...#######\n""##.############..##.\n"".#.######.########.#\n"
            ".###.#######.####.#.\n""#####.##.#.##.###.##\n""..#####..#.#########\n"
            "####################\n""#.####....###.#.#.##\n""##.#################\n"
            "#####.##.###..####..\n""..######..##.#######\n""####.##.####...##..#\n"
            ".#####..#.######.###\n""##...#.##########...\n""#.##########.#######\n"
            ".####.#.###.###.#.##\n""....##.##.###..#####\n"".#.#.###########.###\n"
            "#.#.#.#####.####.###\n""###.##.####.##.#..##\n"
        ),
    };

    std::vector<size_t> outputs = {8, 33, 35, 41, 210};

    for (size_t idx = 0; idx < inputs.size(); ++idx)
    {
        const std::string& input = inputs[idx];
        input_t test1 { const_cast<char*>(input.data()), static_cast<ssize_t>(input.length()) };

        map_t map = convert_map(test1);
        const auto& [asteroid, count] = find_asteroid(map);

        // fmt::print("asteroid: {}, {}, count: {}\n", asteroid.x, asteroid.y, count);
        assert(count == outputs[idx]);
    }
}
