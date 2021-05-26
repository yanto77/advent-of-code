#include "setup/all.h"

ADVENT_DAY(2020, 17, 401, 2224);

namespace
{
    constexpr size_t X = 32, Y = 32, Z = 16, W = 16;
    typedef std::vector<std::vector<bool>> input_world_t;
    typedef std::array<std::array<std::array<bool, X>, Y>, Z> world_pt1_t;
    typedef std::array<std::array<std::array<std::array<bool, X>, Y>, Z>, W> world_pt2_t;

    input_world_t parse_input(const input_t& input)
    {
        input_world_t world;
        parse_input(input, [&](const sv& line)
        {
            std::vector<bool> vec;
            for (char ch: line) vec.push_back(ch == '#');
            world.push_back(vec);
        });

        return world;
    }

    world_pt1_t convert_pt1(const input_world_t& input)
    {
        world_pt1_t output {};

        const size_t height = input.size();
        const size_t width = input[0].size();
        const size_t dx = (X - width) / 2;
        const size_t dy = (Y - height) / 2;
        const size_t dz = Z / 2;

        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++)
                output[dz][dy + y][dx + x] = input[y][x];

        return output;
    }

    world_pt2_t convert_pt2(const input_world_t& input)
    {
        world_pt2_t output {};

        const size_t height = input.size();
        const size_t width = input[0].size();
        const size_t dx = (X - width) / 2;
        const size_t dy = (Y - height) / 2;
        const size_t dz = Z / 2;
        const size_t dw = W / 2;

        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++)
                output[dw][dz][dy + y][dx + x] = input[y][x];

        return output;
    }

    world_pt1_t evaluate_pt1(world_pt1_t world, size_t max_turns)
    {
        world_pt1_t copy{};

        for (size_t turn = 0; turn < max_turns; ++turn)
        {
            for (size_t z = 1; z < Z - 1; z++)
            for (size_t y = 1; y < Y - 1; y++)
            for (size_t x = 1; x < X - 1; x++)
            {
                const bool active = world[z][y][x];
                size_t neighbours = -active;

                for (int dz = -1; dz <= 1; dz++)
                for (int dy = -1; dy <= 1; dy++)
                for (int dx = -1; dx <= 1; dx++)
                {
                    if (world[z + dz][y + dy][x + dx])
                        neighbours++;
                }

                if (active)
                    copy[z][y][x] = (neighbours == 2 || neighbours == 3);
                else
                    copy[z][y][x] = (neighbours == 3);
            }

            world = copy;
        }

        return copy;
    }

    world_pt2_t evaluate_pt2(world_pt2_t world, size_t max_turns)
    {
        world_pt2_t copy{};

        for (size_t turn = 0; turn < max_turns; ++turn)
        {
            for (size_t w = 1; w < W - 1; w++)
            for (size_t z = 1; z < Z - 1; z++)
            for (size_t y = 1; y < Y - 1; y++)
            for (size_t x = 1; x < X - 1; x++)
            {
                const bool active = world[w][z][y][x];
                size_t neighbours = -active;

                for (int dw = -1; dw <= 1; dw++)
                for (int dz = -1; dz <= 1; dz++)
                for (int dy = -1; dy <= 1; dy++)
                for (int dx = -1; dx <= 1; dx++)
                {
                    if (world[w + dw][z + dz][y + dy][x + dx])
                        neighbours++;
                }

                if (active)
                    copy[w][z][y][x] = (neighbours == 2 || neighbours == 3);
                else
                    copy[w][z][y][x] = (neighbours == 3);
            }

            world = copy;
        }

        return copy;
    }

    size_t count_pt1(const world_pt1_t& world)
    {
        size_t sum = 0;
        for (size_t z = 0; z < Z; z++)
        for (size_t y = 0; y < Y; y++)
        for (size_t x = 0; x < X; x++)
        {
            sum += world[z][y][x];
        }
        return sum;
    }

    size_t count_pt2(const world_pt2_t& world)
    {
        size_t sum = 0;
        for (size_t w = 0; w < W; w++)
        for (size_t z = 0; z < Z; z++)
        for (size_t y = 0; y < Y; y++)
        for (size_t x = 0; x < X; x++)
        {
            sum += world[w][z][y][x];
        }
        return sum;
    }
}

output_t Day_2020_17::run_solution(const input_t& input) const
{
    auto world = parse_input(input);
    auto world1 = evaluate_pt1(convert_pt1(world), 6);
    auto world2 = evaluate_pt2(convert_pt2(world), 6);

    size_t part1 = count_pt1(world1);
    size_t part2 = count_pt2(world2);
    return { part1, part2 };
}

void Day_2020_17::run_tests() const
{
    // .#.
    // ..#
    // ###
    input_world_t input(3, std::vector(3, false));
    input[0][1] = input[1][2] = input[2][0] = input[2][1] = input[2][2] = true;

    auto test_world1 = evaluate_pt1(convert_pt1(input), 6);
    assert(count_pt1(test_world1) == 112);

    auto test_world2 = evaluate_pt2(convert_pt2(input), 6);
    assert(count_pt2(test_world2) == 848);
}
