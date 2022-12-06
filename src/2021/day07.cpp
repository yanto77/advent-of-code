#include "setup/all.h"

ADVENT_DAY(2021, 7, 345035, 97038163);

namespace
{
    std::pair<uint32_t, uint32_t> get_min_max(const std::vector<uint32_t>& data)
    {
        uint32_t min = UINT32_MAX;
        uint32_t max = 0;
        for (auto pos: data)
        {
            if (pos < min) min = pos;
            if (pos > max) max = pos;
        }
        return { min, max };
    }

    int32_t get_fuel_part1(const std::vector<uint32_t>& data, int32_t guess)
    {
        int32_t fuel = 0;
        for (uint32_t pos: data)
        {
            fuel += abs(guess - (int32_t)pos);
        }
        return fuel;
    }

    int32_t get_fuel_part2(const std::vector<uint32_t>& data, int32_t guess)
    {
        int32_t fuel = 0;
        for (uint32_t pos: data)
        {
            int32_t n = abs(guess - (int32_t)pos);
            fuel += (n * (n + 1)) / 2;
        }
        return fuel;
    }

    std::pair<int32_t, int32_t> get_best_fuel(std::vector<uint32_t> data)
    {
        struct data_t { int32_t guess; int32_t fuel; };

        data_t part1 = { INT32_MAX, INT32_MAX };
        data_t part2 = { INT32_MAX, INT32_MAX };

        const auto& [min, max] = get_min_max(data);
        for (int32_t i = min; i <= max; i++)
        {
            int32_t fuel_pt1 = get_fuel_part1(data, i);
            if (fuel_pt1 < part1.fuel)
            {
                part1 = { i, fuel_pt1 };
            }

            int32_t fuel_pt2 = get_fuel_part2(data, i);
            if (fuel_pt2 < part2.fuel)
            {
                part2 = { i, fuel_pt2 };
            }
        }

        return { part1.fuel, part2.fuel };
    }
}

result_t Day_2021_7::run_solution(str_view input) const
{
    std::vector<uint32_t> data;
    parse_uint_scalar(input, [&data](uint32_t num)
    {
        data.push_back(num);
    });
    const auto& [part1, part2] = get_best_fuel(data);
    return { static_cast<size_t>(part1), static_cast<size_t>(part2) };
}

void Day_2021_7::run_tests() const
{
    char text1[] = "16,1,2,0,4,2,7,1,2,14\n";

    std::vector<uint32_t> data;
    parse_uint_scalar(text1, [&data](uint32_t num)
    {
        data.push_back(num);
    });

    const auto& [part1, part2] = get_best_fuel(data);

    assert(part1 == 37);
    assert(part2 == 168);
    assert(get_fuel_part1(data, 1) == 41);
    assert(get_fuel_part1(data, 3) == 39);
    assert(get_fuel_part1(data, 10) == 71);
    assert(get_fuel_part2(data, 2) == 206);
}
