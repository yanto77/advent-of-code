#include "setup/all.h"
#include <unordered_map>

ADVENT_DAY(2022, 8, 1816, 383520);

namespace 
{
    // Iteration: sequential/predictable is good, jumping around is bad.
    // ref: https://stackoverflow.com/q/48781358

    template <size_t Y, size_t X>
    struct map_t {
        std::array<uint8_t, Y*X> normal;
    };

    template <size_t Y, size_t X>
    map_t<Y, X> parse_map(str_view input)
    {
        map_t<Y, X> map {};

        for (size_t y = 0; y < Y; y++)
        {
            for (size_t x = 0; x < X; x++)
            {
                char ch = input[y*(X+1) + x]; // (+1) is for the '\n'
                uint8_t height = parse_uint(ch);
                map.normal[y*X + x] = height;
            }
        }

        return map;
    }

    template <size_t Y, size_t X>
    std::array<bool, Y*X> get_visible(map_t<Y, X> map)
    {
        std::array<bool, Y*X> visible {};

        // Horizontal forward pass
        for (int64_t y = 0; y < Y; y++)
        {
            int8_t max = -1;
            for (int64_t x = 0; x < X; x++)
            {
                uint8_t curr = map.normal[y*(X) + x];
                if (max < curr)
                {
                    visible[y*(X) + x] = true;
                    max = curr;
                }
            }
        }

        // Horizontal backwards pass
        for (int64_t y = Y - 1; y >= 0; y--)
        {
            int8_t max = -1;
            for (int64_t x = X - 1; x >= 0; x--)
            {
                uint8_t curr = map.normal[y*(X) + x];
                if (max < curr)
                {
                    visible[y*(X) + x] = true;
                    max = curr;
                }
            }
        }

        // Vertical forward pass
        for (int64_t x = 0; x < X; x++)
        {
            int8_t max = -1;
            for (int64_t y = 0; y < Y; y++)
            {
                uint8_t curr = map.normal[y*X + x];
                if (max < curr)
                {
                    visible[y*X + x] = true;
                    max = curr;
                }
            }
        }

        // Vertical backwards pass
        for (int64_t x = X - 1; x >= 0; x--)
        {
            int8_t max = -1;
            for (int64_t y = Y - 1; y >= 0; y--)
            {
                uint8_t curr = map.normal[y*X + x];
                if (max < curr)
                {
                    visible[y*X + x] = true;
                    max = curr;
                }
            }
        }

        return visible;
    }

    template <size_t Y_MAX, size_t X_MAX>
    size_t get_max_scenic_score(map_t<Y_MAX, X_MAX> map)
    {
        size_t max_score = 0;
        for (int64_t y = 0; y < Y_MAX; y++)
        {
            for (int64_t x = 0; x < X_MAX; x++)
            {
                const uint8_t curr = map.normal[y*X_MAX + x];
                size_t counts[4] = { 0, 0, 0, 0 };
                
                // left
                for (int64_t x2 = x - 1; x2 >= 0; x2--)
                {
                    counts[0]++;
                    uint8_t next = map.normal[y*X_MAX + x2];
                    if (next >= curr)
                        break;
                }

                // right
                for (int64_t x2 = x + 1; x2 < X_MAX; x2++)
                {
                    counts[1]++;
                    uint8_t next = map.normal[y*X_MAX + x2];
                    if (next >= curr)
                        break;
                }

                // top
                for (int64_t y2 = y - 1; y2 >= 0; y2--)
                {
                    counts[2]++;
                    uint8_t next = map.normal[y2*X_MAX + x];
                    if (next >= curr)
                        break;
                }

                // bottom
                for (int64_t y2 = y + 1; y2 < Y_MAX; y2++)
                {
                    counts[3]++;
                    uint8_t next = map.normal[y2*X_MAX + x];
                    if (next >= curr)
                        break;
                }
            
                size_t cell_score = counts[0] * counts[1] * counts[2] * counts[3];
                if (cell_score > max_score)
                {
                    max_score = cell_score;
                }
            }
        }

        return max_score;
    }
}

result_t Day_2022_8::run_solution(str_view input) const
{
    auto map = parse_map<99, 99>(input);
    auto visible_map = get_visible(map);

    size_t part1 = 0;
    for (bool visible_cell: visible_map)
    {
        part1 += (int)visible_cell;
    }

    size_t part2 = get_max_scenic_score(map);
    return { part1, part2 };
}

void Day_2022_8::run_tests() const
{
    std::string input =
        "30373\n"
        "25512\n"
        "65332\n"
        "33549\n"
        "35390\n";
    auto map = parse_map<5, 5>(input);
    auto visible_map = get_visible(map);

    size_t part1 = 0;
    for (bool visible_cell: visible_map)
    {
        part1 += (int)visible_cell;
    }
    assert(part1 == 21);

    size_t part2 = get_max_scenic_score(map);
    assert(part2 == 8);
}
