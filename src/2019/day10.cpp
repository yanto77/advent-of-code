#include "setup/all.h"
#include <unordered_set>

ADVENT_DAY(2019, 10, 286, 504);

namespace
{
    typedef std::vector<std::vector<bool>> map_t;

    map_t convert_map(str_view input)
    {
        map_t out;

        parse_input(input, [&](str_view line_str)
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

    std::pair<uint8_t, float> get_quad_slope(const vec2i& a, const vec2i& b)
    {
        vec2i delta = (a - b);
        float x = static_cast<float>(delta.x);
        float y = static_cast<float>(delta.y);
        float slope = y / x;
        uint8_t bucket = std::signbit(y) * 2 + std::signbit(x);
        return {bucket, slope};
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

                const auto& [quadrant, slope] = get_quad_slope(asteroid1, asteroid2);
                deltas[quadrant].insert(slope);
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

    // Destroy asteroids around the `station`, return 200th (or last) asteroid location
    //
    // NB: single laser rotation is enough to destroy 200+ asteroids (or max amount)
    // in the tests and inputs. As such, that part is not implemented.
    size_t clear_asteroids(const map_t& map, const vec2i& station)
    {

        std::array<std::map<float, vec2i>, 4> closest; // for each quadrant, sorted map of {slope -> asteroid }
        for_each_asteroid(map, [&](const vec2i& asteroid)
        {
            if (asteroid == station)
                return;

            const auto& [quad, slope] = get_quad_slope(asteroid, station);

            std::map<float, vec2i>& quadrant = closest[quad];
            if (auto it = quadrant.find(slope); it != quadrant.end())
            {
                // if there is already an asteroid at this slope, pick the closest one
                int32_t new_to_st = (asteroid - station).norm_sq();
                int32_t old_to_st = (it->second - station).norm_sq();
                if (new_to_st < old_to_st)
                {
                    it->second = asteroid;
                }
            }
            else
            {
                quadrant.insert({ slope, asteroid });
            }
        });

        // quads are arranged as follows:
        //   1   0
        //     .
        //   3   2
        size_t counter = 0;
        for (auto quad: {2, 0, 1, 3})
        {
            for (auto& [slope, asteroid]: closest[quad])
            {
                ++counter;
                // map[asteroid.y][asteroid.x] = false; // BOOM!

                if (counter == 200)
                {
                    return asteroid.x * 100 + asteroid.y;
                }
            }
        }

        return 0;
    }
}

result_t Day_2019_10::run_solution(str_view input) const
{
    map_t map = convert_map(input);
    const auto& [asteroid, part1] = find_asteroid(map);

    size_t part2 = clear_asteroids(map, asteroid);

    return {part1, part2};
}

void Day_2019_10::run_tests() const
{
    std::vector<std::string> inputs = 
    {
        ".#..#\n"".....\n""#####\n""....#\n""...##\n",
        
        "......#.#.\n""#..#.#....\n""..#######.\n"".#.#.###..\n"".#..#.....\n"
        "..#....#.#\n""#..#....#.\n"".##.#..###\n""##...#..#.\n"".#....####\n",
    
        "#.#...#.#.\n"".###....#.\n"".#....#...\n""##.#.#.#.#\n""....#.#.#.\n"
        ".##..###.#\n""..#...##..\n""..##....##\n""......#...\n"".####.###.\n",
    
        ".#..#..###\n""####.###.#\n""....###.#.\n""..###.##.#\n""##.##.#.#.\n"
        "....###..#\n""..#.#..#.#\n""#..#.#.###\n"".##...##.#\n"".....#.#..\n",
        
        ".#..##.###...#######\n""##.############..##.\n"".#.######.########.#\n"
        ".###.#######.####.#.\n""#####.##.#.##.###.##\n""..#####..#.#########\n"
        "####################\n""#.####....###.#.#.##\n""##.#################\n"
        "#####.##.###..####..\n""..######..##.#######\n""####.##.####...##..#\n"
        ".#####..#.######.###\n""##...#.##########...\n""#.##########.#######\n"
        ".####.#.###.###.#.##\n""....##.##.###..#####\n"".#.#.###########.###\n"
        "#.#.#.#####.####.###\n""###.##.####.##.#..##\n",
    };

    std::vector<size_t> outputs = {8, 33, 35, 41, 210};

    for (size_t idx = 0; idx < inputs.size(); ++idx)
    {
        map_t map = convert_map(inputs[idx]);
        const auto& [asteroid, count] = find_asteroid(map);

        // fmt::print("asteroid: {}, {}, count: {}\n", asteroid.x, asteroid.y, count);
        assert(count == outputs[idx]);

        clear_asteroids(map, asteroid);
    }
}
