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

    void for_each_asteroid(const map_t& map, std::function<void(const vec2i&)> callback)
    {
        const size_t y_dim = map.size();
        const size_t x_dim = map[0].size();

        for (int32_t y = 0; y < y_dim; ++y)
            for (int32_t x = 0; x < x_dim; ++x)
                if (map[y][x])
                    callback(vec2i { x, y });
    }

    // Find the coordinates for Monitoring station (part 1)
    std::pair<vec2i, size_t> find_asteroid(const map_t& map)
    {
        std::unordered_map<vec2i, size_t> counts;

        for_each_asteroid(map, [&](const vec2i& asteroid1)
        {
            std::array<std::unordered_set<float>, 4> deltas;
            for_each_asteroid(map, [&](const vec2i& asteroid2)
            {
                if (asteroid1 == asteroid2)
                    return;

                const vec2i delta = (asteroid1 - asteroid2);
                float x = static_cast<float>(delta.x);
                float y = static_cast<float>(delta.y);
                float slope = y / x;

                uint8_t bucket = std::signbit(x) * 2 + std::signbit(y);
                deltas[bucket].insert(slope);
            });

            counts[asteroid1] = 0;
            for (const auto& bucket: deltas)
            {
                counts[asteroid1] += bucket.size();
            }
        });

        auto max = std::max_element(std::begin(counts), std::end(counts),
            [] (const auto& p1, const auto& p2) { return p1.second < p2.second; });

        return { max->first, max->second };
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
