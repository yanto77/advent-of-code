#include "setup/all.h"
#include <optional>
#include <unordered_set>

ADVENT_DAY(2022, 15, 5367037, 11914583249288);

namespace 
{
    struct bound_t 
    {
        vec2i min {INT32_MAX, INT32_MAX};
        vec2i max {INT32_MIN, INT32_MIN};

        void add(vec2i p)
        {
            min.x = ::min(min.x, p.x);
            min.y = ::min(min.y, p.y);
            max.x = ::max(max.x, p.x);
            max.y = ::max(max.y, p.y);
        }
    };

    struct sensor_t { vec2i center; int64_t extent; };
    
    struct sensor_map_t 
    {
        bound_t outer_bound;
        bound_t inner_bound;
        std::vector<sensor_t> sensors;
        std::unordered_set<vec2i> beacons;

        sensor_map_t(str_view input)
        {
            for_each_split(input, '\n', [&](str_view line)
            {
                auto [sx, next1] = to_int<int32_t>(line, sizeof("Sensor at x=")-1);
                auto [sy, next2] = to_int<int32_t>(line, next1 + sizeof(", y=")-1);
                auto [bx, next3] = to_int<int32_t>(line, next2 + sizeof(": closest beacon is at x=")-1);
                auto [by, next4] = to_int<int32_t>(line, next3 + sizeof(", y=")-1);

                vec2i sensor {sx, sy};
                vec2i beacon {bx, by};
                int32_t extent = (int32_t)dist_mnht(sensor, beacon);
                sensors.push_back({ sensor, extent });
                beacons.insert({ beacon });

                outer_bound.add(vec2i { sx - extent, sy - extent });
                outer_bound.add(vec2i { sx + extent, sy + extent });
                inner_bound.add(vec2i { sx, sy });
            });
        }

        size_t count_covered_cells(int32_t y)
        {
            size_t count = 0;
            std::vector<vec2i> xranges;
            xranges.reserve(sensors.size());
            compute_ranges_x(y, outer_bound, xranges);

            for (auto [x1, x2]: xranges)
            {
                count += (x2 - x1) + 1; // NB: incl. range
                for (vec2i beacon: beacons)
                {
                    if (y == beacon.y && x1 <= beacon.x && beacon.x <= x2)
                    {
                        count--;
                    }
                }
            }

            return count;
        }

        size_t find_beacon_score()
        {
            std::vector<vec2i> xranges, yranges;
            xranges.reserve(sensors.size());
            yranges.reserve(sensors.size());
            
            compute_ranges_y(inner_bound, yranges);
            for (auto [min, max]: yranges)
            {
                for (int32_t y = min; y < max; y++)
                {
                    compute_ranges_x(y, outer_bound, xranges);
                    if (xranges.size() == 2)
                    {
                        const vec2i a = xranges[0], b = xranges[1];
                        if (a.x > b.y) return int64_t(b.y + 1) * 4000000 + y;
                        else return int64_t(a.y + 1) * 4000000 + y;
                    }
                }
            }

            assert(false);
            return 0;
        }

        [[maybe_unused]]
        void print(std::optional<vec2i> center = {}, std::vector<vec2i> highlights = {})
        {
            auto [min, max] = outer_bound;
            if (center.has_value())
            {
                min.x = center.value().x - 5;
                min.y = center.value().y - 5;
                max.x = center.value().x + 5;
                max.y = center.value().y + 5;
            }

            for (int32_t y = min.y; y <= max.y; y++)
            {
                fmt::print("{:5} : ", y);
                for (int32_t x = min.x; x <= max.x; x++)
                {
                    const vec2i pos { x, y };

                    // Layer 1: Beacons and Sensors
                    for (auto beacon: beacons) {
                        if (pos == beacon) {
                            fmt::print(COLOR_GREEN() "B");
                            goto next;
                        }
                    }
                    for (auto [sensor, extent]: sensors) {
                        if (pos == sensor) { 
                            fmt::print(COLOR_BLUE() "S");
                            goto next;
                        }
                    }

                    // Layer 2: High-lighted areas
                    for (auto [sensor, extent]: sensors) 
                    {
                        if (dist_mnht(sensor, pos) <= extent) // in range
                        {
                            for (auto highlight: highlights) 
                            {
                                if (sensor == highlight) 
                                {
                                    fmt::print(COLOR_CYAN() "#");
                                    goto next;
                                }
                            }
                        }
                    }

                    // Layer 3: Sensor areas (not highlighted)
                    for (auto [sensor, extent]: sensors) 
                    {
                        if (dist_mnht(sensor, pos) <= extent) // in range
                        {
                            fmt::print(COLOR_GRAY() "o");
                            goto next;
                        }
                    }

                    // Layer 4: Empty locations
                    fmt::print(COLOR_GRAY() ".");

                    next:
                        continue;
                }
                fmt::print(COLOR_RESET() "\n");
            }
        }

        private:
            void compute_ranges_x(int32_t y, const bound_t& bound, std::vector<vec2i>& ranges)
            {
                ranges.clear();
                for (auto [center, extent]: sensors)
                {
                    int32_t extent_in_y = (int32_t)extent - abs(y - center.y);
                    if (extent_in_y > 0)
                    {
                        int32_t x1 = clamp(center.x - extent_in_y, bound.min.x, bound.max.x);
                        int32_t x2 = clamp(center.x + extent_in_y, bound.min.x, bound.max.x);
                        ranges.push_back(vec2i{x1, x2});
                    }
                }
                merge_ranges(ranges);
            }

            void compute_ranges_y(const bound_t& bound, std::vector<vec2i>& ranges)
            {
                ranges.clear();
                for (size_t i = 0; i < sensors.size(); i++)
                {
                    for (size_t j = 0; j < i; j++)
                    {
                        auto [ap, ad] = sensors[i];
                        auto [bp, bd] = sensors[j];
                        if (dist_mnht(ap, bp) - bd - ad == 2)
                        {
                            // There's a 1 cell diagonal between the sensor areas where the missing sensor could be found
                            int32_t ymin = clamp(min(ap.y, bp.y), bound.min.y, bound.max.y);
                            int32_t ymax = clamp(max(ap.y, bp.y), bound.min.y, bound.max.y);
                            ranges.push_back({ymin, ymax});
                        }
                    }
                }
                merge_ranges(ranges);
            }

            void merge_ranges(std::vector<vec2i>& ranges)
            {
                for (int64_t i = ranges.size() - 1; i >= 0; i--)
                {
                    for (int64_t j = i - 1; j >= 0; j--)
                    {
                        const vec2i a = ranges[i], b = ranges[j];
                        if (!((b.x - 1) > a.y || (a.x - 1) > b.y))
                        {
                            ranges[j] = { min(a.x, b.x), max(a.y, b.y) };
                            ranges[i] = std::move(ranges.back());
                            ranges.pop_back();
                            break;
                        }
                    }
                }
            }
    };
}

result_t Day_2022_15::run_solution(str_view input) const
{
    sensor_map_t map(input);
    size_t part1 = map.count_covered_cells(2000000);
    size_t part2 = map.find_beacon_score();
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

    sensor_map_t map(test_input);
    size_t part1 = map.count_covered_cells(10);
    assert(part1 == 26);

    // map.print();
    size_t part2 = map.find_beacon_score();
    assert(part2 == 56000011);
}
