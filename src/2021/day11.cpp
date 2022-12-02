#include "setup/all.h"

ADVENT_DAY(2021, 11, 1747, 505);

namespace
{
    constexpr std::array<vec2i8, 8> directions =
    {
        vec2i8{-1, -1}, vec2i8{0, -1}, vec2i8{1, -1},
        vec2i8{-1,  0},                vec2i8{1,  0},
        vec2i8{-1,  1}, vec2i8{0,  1}, vec2i8{1,  1},
    };

    constexpr size_t PAD = 1;
    typedef std::array<std::array<uint8_t, 10 + 2 * PAD>, 10 + 2 * PAD> map_t;

    [[maybe_unused]]
    void print_map(const map_t& map)
    {
        clear_screen();
        fmt::print("map:\n");

        std::unordered_map<uint8_t, const char*> colors = {
            { 0, COLOR_WHITE() },
            { 1, TRUE_COLOR(30, 30, 30) },
            { 2, TRUE_COLOR(60, 60, 60) },
            { 3, TRUE_COLOR(90, 90, 90) },
            { 4, TRUE_COLOR(120, 120, 120) },
            { 5, TRUE_COLOR(150, 150, 150) },
            { 6, TRUE_COLOR(180, 180, 180) },
            { 7, TRUE_COLOR(210, 210, 210) },
            { 8, TRUE_COLOR(240, 240, 240) },
            { 9, COLOR_YELLOW() },
        };

        for (int y = 0 + PAD; y < 10 + PAD; y++)
        {
            for (int x = 0 + PAD; x < 10 + PAD; x++)
            {
                uint8_t num = map[y][x];

                if (num > 9)
                {
                    fmt::print(COLOR_RED() "9");
                }
                else
                {
                    fmt::print(fmt::runtime(colors[num]));
                    fmt::print("{}", num);
                }
            }
            fmt::print("\n");
        }
        fmt::print(COLOR_RESET());
    }

    map_t parse_map(str_view input)
    {
        map_t map {};

        const char* read_ptr = input.data();
        for (int y = 0 + PAD; y < 10 + PAD; y++)
        {
            for (int x = 0 + PAD; x < 10 + PAD; x++)
            {
                map[y][x] = to_int<uint8_t>(*read_ptr);
                read_ptr++;
            }

            read_ptr++;
        }

        return map;
    }

    // return number of flashes during this step
    size_t evolve_step(map_t& map)
    {
        size_t flashes = 0;

        // base update
        for (int y = 0 + PAD; y < 10 + PAD; y++)
        {
            for (int x = 0 + PAD; x < 10 + PAD; x++)
            {
                map[y][x]++;
            }
        }

        // update flashes
        while (true)
        {
            bool stop = true;
            for (int y = 0 + PAD; y < 10 + PAD; y++)
            {
                for (int x = 0 + PAD; x < 10 + PAD; x++)
                {
                    if (map[y][x] == 10)
                    {
                        flashes++;
                        map[y][x]++;
                        stop = false;

                        for (auto dir: directions)
                        {
                            uint8_t& val = map[y + dir.y][x + dir.x];
                            if (val != 10) // don't skip flashes
                                val++;
                        }
                    }
                }
            }

            if (stop) break;
        }

        // reset padding and over-flashed numbers
        for (int y = 0; y < 10 + PAD * 2; y++)
        {
            for (int x = 0; x < 10 + PAD * 2; x++)
            {
                bool padding = (x == 0 || x == 11 || y == 0 || y == 11);
                bool flashed = (map[y][x] > 9);

                if (padding || flashed)
                    map[y][x] = 0;
            }
        }

        return flashes;
    }

    std::pair<size_t, size_t> solve(str_view input)
    {
        size_t flashes = 0;
        size_t step = 0;

        map_t map = parse_map(input);

        while (step < 100)
        {
            flashes += evolve_step(map);
            step++;
        }

        while (true)
        {
            evolve_step(map);
            step++;

            bool stop = true;
            for (auto& row: map)
                for (uint8_t num: row)
                    if (num != 0)
                        stop = false;

            // print_map(map);
            // std::this_thread::sleep_for(10ms);

            if (stop)
                break;
        }

        return { flashes, step };
    }
}

result_t Day_2021_11::run_solution(str_view input) const
{
    const auto& [part1, part2] = solve(input);
    return { part1, part2 };
}

void Day_2021_11::run_tests() const
{
    char text1[] =
        "5483143223\n"
        "2745854711\n"
        "5264556173\n"
        "6141336146\n"
        "6357385478\n"
        "4167524645\n"
        "2176841721\n"
        "6882881134\n"
        "4846848554\n"
        "5283751526\n"
        "";

    const auto& [part1, part2] = solve(text1);
    // fmt::print("test part1: {}, part2: {}\n", part1, part2);
    assert(part1 == 1656);
    assert(part2 == 195);
}
