#include "setup/all.h"
#include <optional>

ADVENT_DAY(2022, 15, 5367037, 0);

namespace 
{
    struct sensor_t 
    {
        vec2i pos {};
        vec2i beacon {};
    };

    struct bound_t 
    {
        vec2i min {INT32_MAX, INT32_MAX};
        vec2i max {INT32_MIN, INT32_MIN};
    };

    std::vector<sensor_t> parse_sensors(str_view input)
    {
        std::vector<sensor_t> result;

        for_each_split(input, '\n', [&](str_view line)
        {
            // fmt::print("line: {}\n", line);
            auto [sx, next1] = to_int<int32_t>(line, sizeof("Sensor at x=")-1);
            auto [sy, next2] = to_int<int32_t>(line, next1 + sizeof(", y=")-1);
            auto [bx, next3] = to_int<int32_t>(line, next2 + sizeof(": closest beacon is at x=")-1);
            auto [by, next4] = to_int<int32_t>(line, next3 + sizeof(", y=")-1);
            // fmt::print(" - {}, {}, {}, {}\n", sx, sy, bx, by);
            result.push_back(sensor_t{ vec2i{sx, sy}, vec2i{bx, by} });
        });

        return result;
    }

    bound_t get_total_bound(const std::vector<sensor_t>& sensors)
    {
        bound_t total {};
        for (auto [sensor, beacon]: sensors)
        {
            int32_t dist = (int32_t)dist_mnht(sensor, beacon);
            vec2i p1 { sensor.x - dist, sensor.y - dist };
            total.min.x = min(total.min.x, p1.x);
            total.min.y = min(total.min.y, p1.y);
            vec2i p2 { sensor.x + dist, sensor.y + dist };
            total.max.x = max(total.max.x, p2.x);
            total.max.y = max(total.max.y, p2.y);
        }

        fmt::print("bound: min {} {}, max: {} {}\n", total.min.x, total.min.y, total.max.x, total.max.y);
        return total;
    }

    bool is_cell_covered(const std::vector<sensor_t>& sensors, vec2i pos)
    {
        for (auto [sensor, beacon]: sensors)
        {
            if (pos == sensor || pos == beacon)
                continue;

            if (dist_mnht(sensor, pos) <= dist_mnht(sensor, beacon))
                return true;
        }
        
        return false;
    }

    void print_map(const std::vector<sensor_t>& sensors, std::optional<vec2i> center = {})
    {
        auto [min, max] = get_total_bound(sensors);
        if (center.has_value())
        {
            min.x = center.value().x - 5;
            min.y = center.value().y - 5;
            max.x = center.value().x + 5;
            max.y = center.value().y + 5;
        }

        for (int32_t y = min.y; y <= max.y; y++)
        {
            for (int32_t x = min.x; x <= max.x; x++)
            {
                vec2i pos { x, y };
                bool found_sensor = false;
                bool found_beacon = false;
                bool in_range = false;

                for (auto [sensor, beacon]: sensors)
                {
                    if (pos == sensor) found_sensor = true;
                    if (pos == beacon) found_beacon = true;

                    // Print sensor range
                    int32_t dist1 = (int32_t)dist_mnht(sensor, pos);
                    int32_t dist2 = (int32_t)dist_mnht(sensor, beacon);
                    in_range |= dist1 <= dist2;

                    // Print sensor range AABB
                    // vec2i p1 { sensor.x - dist2, sensor.y - dist2 };
                    // vec2i p2 { sensor.x + dist2, sensor.y + dist2 };
                    // in_range |= (p1 <= pos && pos <= p2);
                }

                if (found_sensor)       fmt::print(COLOR_BLUE() "S");
                else if (found_beacon)  fmt::print(COLOR_GREEN() "B");
                else if (in_range)      fmt::print(COLOR_WHITE() "#");
                else                    fmt::print(COLOR_GRAY() ".");
            }
            fmt::print(COLOR_RESET() "\n");
        }
    }

    // return number of occupied cells across single y-line
    size_t count_line_pos(const std::vector<sensor_t>& sensors, int32_t y)
    {
        size_t n_pos = 0;

        auto [min, max] = get_total_bound(sensors);
        for (int32_t x = min.x; x <= max.x; x++)
        {
            n_pos += (int)is_cell_covered(sensors, {x, y});
        }

        return n_pos;
    }

    // find beacon pos (unoccupied cell in between of all sensor-areas)
    vec2i find_beacon(const std::vector<sensor_t>& sensors)
    {
        auto [min, max] = get_total_bound(sensors);
        min.x = ::max(min.x, 0);
        min.y = ::max(min.y, 0);
        max.x = ::min(max.x, 4000000);
        max.y = ::min(max.y, 4000000);
        // fmt::print("test part2: {}, pos: {}, {}\n", part2, asd.x, asd.y);

        for (int32_t y = min.y; y <= max.y; y++)
        {
            for (int32_t x = min.x; x <= max.x; x++)
            {
                fmt::print("x%: {:2.3f}, y%: {:2.3f}\n", (float)x / (max.x - min.x), (float)y / (max.y - min.y));
                // 0 1 2
                // 3 4 5
                // 6 7 8
                bool cell0 = is_cell_covered(sensors, {x-1, y-1});
                bool cell1 = is_cell_covered(sensors, {x+0, y-1});
                bool cell2 = is_cell_covered(sensors, {x+1, y-1});
                bool cell3 = is_cell_covered(sensors, {x-1, y+0});
                bool cell4 = is_cell_covered(sensors, {x+0, y+0});
                bool cell5 = is_cell_covered(sensors, {x+1, y+0});
                bool cell6 = is_cell_covered(sensors, {x-1, y+1});
                bool cell7 = is_cell_covered(sensors, {x+0, y+1});
                bool cell8 = is_cell_covered(sensors, {x+1, y+1});

                bool beacon = 
                    cell0 && cell1 && cell2 &&
                    cell3 && !cell4 && cell5 &&
                    cell6 && cell7 && cell8;

                if (beacon)
                {
                    bool taken = false;
                    for (auto [sensor, beacon]: sensors)
                    {
                        if (sensor == vec2i{x, y} || beacon == vec2i{x, y}) 
                        {
                            taken = true;
                            break;
                        }
                    }

                    if (!taken)
                    {
                        print_map(sensors, {{x, y}});
                        return {x, y};
                    }
                }
            }
        }

        return {0, 0};
    }
}

// TODO: add bvh for log-n checks?
result_t Day_2022_15::run_solution(str_view input) const
{
    size_t part1 = 0;
    size_t part2 = 0;

    // std::vector<sensor_t> sensors = parse_sensors(input);
    // // print_map(sensors);

    // part1 = count_line_pos(sensors, 2000000);
    // auto asd = find_beacon(sensors);
    // part2 = 4000000 * asd.x + asd.y;

    // must have x and y coordinates each no lower than 0 and no larger than 4000000.
    return { part1, part2 };
}

void Day_2022_15::run_tests() const
{

    std::string test_input = 
        "Sensor at x=2, y=18: closest beacon is at x=-2, y=15\n"
        "Sensor at x=9, y=16: closest beacon is at x=10, y=16\n"
        "Sensor at x=13, y=2: closest beacon is at x=15, y=3\n"
        "Sensor at x=12, y=14: closest beacon is at x=10, y=16\n"
        "Sensor at x=10, y=20: closest beacon is at x=10, y=16\n"
        "Sensor at x=14, y=17: closest beacon is at x=10, y=16\n"
        "Sensor at x=8, y=7: closest beacon is at x=2, y=10\n"
        "Sensor at x=2, y=0: closest beacon is at x=2, y=10\n"
        "Sensor at x=0, y=11: closest beacon is at x=2, y=10\n"
        "Sensor at x=20, y=14: closest beacon is at x=25, y=17\n"
        "Sensor at x=17, y=20: closest beacon is at x=21, y=22\n"
        "Sensor at x=16, y=7: closest beacon is at x=15, y=3\n"
        "Sensor at x=14, y=3: closest beacon is at x=15, y=3\n"
        "Sensor at x=20, y=1: closest beacon is at x=15, y=3\n";

    std::vector<sensor_t> sensors = parse_sensors(test_input);
    // print_map(sensors);

    size_t part1 = count_line_pos(sensors, 10);
    assert(part1 == 26);
    fmt::print("test part1: {}\n", part1);

    auto asd = find_beacon(sensors);
    size_t part2 = 4000000 * asd.x + asd.y;
    fmt::print("test part2: {}, pos: {}, {}\n", part2, asd.x, asd.y);
    assert(part2 == 56000011);
}
