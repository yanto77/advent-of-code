#include "setup/all.h"

ADVENT_DAY(2019, 3, 1519, 0);

namespace
{
    typedef std::vector<vec2i> polyline_t;
    typedef std::array<vec2i, 2> segment_t;

    // Ref: https://stackoverflow.com/a/35474310
    constexpr bool intersect(const segment_t& s1, const segment_t& s2, vec2i& point)
    {
        int32_t ax = s1[1].x - s1[0].x;     // direction of line a
        int32_t ay = s1[1].y - s1[0].y;     // ax and ay as above

        int32_t bx = s2[0].x - s2[1].x;     // direction of line b, reversed
        int32_t by = s2[0].y - s2[1].y;     // really -by and -by as above

        int32_t dx = s2[0].x - s1[0].x;   // right-hand side
        int32_t dy = s2[0].y - s1[0].y;

        float det = static_cast<float>(ax * by - ay * bx);

        if (det == 0.0f) return false;

        float r = static_cast<float>(dx * by - dy * bx) / det;
        float s = static_cast<float>(ax * dy - ay * dx) / det;

        if (!(r < 0 || r > 1 || s < 0 || s > 1))
        {
            point.x = s1[0].x + static_cast<int32_t>(r * static_cast<float>((s1[1].x - s1[0].x)));
            point.y = s1[0].y + static_cast<int32_t>(r * static_cast<float>((s1[1].y - s1[0].y)));

            // point.x = s1[0].x + static_cast<int32_t>(s * static_cast<float>((s1[1].x - s1[0].x)));
            // point.y = s1[0].y + static_cast<int32_t>(s * static_cast<float>((s1[1].y - s1[0].y)));
        }

        return !(r < 0 || r > 1 || s < 0 || s > 1);
    }

    polyline_t get_polyline(const sv& line)
    {
        polyline_t out = { vec2i{0, 0} };
        split_multi(line, ',', [&](const sv& part)
        {
            const auto& [num, end] = to_int<uint16_t>(part, 1);
            switch (part[0]) // R right, U up, D down, L left
            {
                case 'R': out.push_back(out.back() + vec2i{num, 0}); break;
                case 'L': out.push_back(out.back() + vec2i{-num, 0}); break;
                case 'U': out.push_back(out.back() + vec2i{0, num}); break;
                case 'D': out.push_back(out.back() + vec2i{0, -num}); break;
                default: break;
            }
        });

        return out;
    }

    // get manhattan distance from {0, 0}
    size_t dist(const vec2i& in)
    {
        return abs(in.x) + abs(in.y);
    }

    vec2i get_closest_intersection(const polyline_t& line1, const polyline_t& line2)
    {
        vec2i closest_point { INT_MAX / 2, INT_MAX / 2 };
        
        for (size_t idx1 = 0; idx1 < line1.size() - 1; ++idx1)
        {
            segment_t s1 { line1[idx1], line1[idx1+1] };
            for (size_t idx2 = 0; idx2 < line2.size() - 1; ++idx2)
            {
                segment_t s2 { line2[idx2], line2[idx2+1] };

                vec2i point;
                if (intersect(s1, s2, point))
                {
                    // fmt::print("intersection! s1: [{}, {}], [{}, {}], s2: [{}, {}], [{}, {}]\n", 
                    // s1[0].x, s1[0].y, s1[1].x, s1[1].y, 
                    // s2[0].x, s2[0].y, s2[1].x, s2[1].y);

                    // fmt::print(" -> point: [{}, {}], dist: {}\n", point.x, point.y, dist(point));

                    if (point != vec2i{0, 0} && dist(point) < dist(closest_point))
                    {
                        closest_point = point;
                    }
                }
            }
        }

        return closest_point;
    }
}

output_t Day_2019_3::run_solution(const input_t& input) const
{
    std::vector<polyline_t> lines;

    parse_input(input, [&](const sv& line)
    {
        lines.push_back(get_polyline(line));
    });

    auto point = get_closest_intersection(lines[0], lines[1]);
    // fmt::print("point 1: {}, {}\n", point.x, point.y);

    return {dist(point), 0};
}

void Day_2019_3::run_tests() const
{
    std::vector<std::string> inputs = {
        "R8,U5,L5,D3",
        "U7,R6,D4,L4",

        "R75,D30,R83,U83,L12,D49,R71,U7,L72",
        "U62,R66,U55,R34,D71,R55,D58,R83",

        "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51",
        "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7",
    };

    {
        auto line1 = get_polyline(sv{ inputs[0].data(), inputs[0].length() });
        auto line2 = get_polyline(sv{ inputs[1].data(), inputs[1].length() });
        auto point = get_closest_intersection(line1, line2);
        // fmt::print("point 1: {}, {}\n", point.x, point.y);
        assert(dist(point) == 6);
    }

    {
        auto line1 = get_polyline(sv{ inputs[2].data(), inputs[2].length() });
        auto line2 = get_polyline(sv{ inputs[3].data(), inputs[3].length() });
        auto point = get_closest_intersection(line1, line2);
        // fmt::print("point 2: {}, {}\n", point.x, point.y);
        assert(dist(point) == 159);
    }

    {
        auto line1 = get_polyline(sv{ inputs[4].data(), inputs[4].length() });
        auto line2 = get_polyline(sv{ inputs[5].data(), inputs[5].length() });
        auto point = get_closest_intersection(line1, line2);
        // fmt::print("point 3: {}, {}\n", point.x, point.y);
        assert(dist(point) == 135);
    }
}
