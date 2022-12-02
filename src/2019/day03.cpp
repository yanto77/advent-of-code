#include "setup/all.h"

ADVENT_DAY(2019, 3, 1519, 14358);

namespace
{
    typedef std::vector<vec2i> polyline_t;
    typedef std::array<vec2i, 2> segment_t;
    typedef std::vector<std::pair<vec2i, size_t>> cross_list_t;

    // Ref: https://en.wikipedia.org/wiki/Intersection_(Euclidean_geometry)#Two_line_segments
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

        bool hit = !(r < 0 || r > 1 || s < 0 || s > 1);
        if (hit)
        {
            point.x = s1[0].x + static_cast<int32_t>(r * static_cast<float>((s1[1].x - s1[0].x)));
            point.y = s1[0].y + static_cast<int32_t>(r * static_cast<float>((s1[1].y - s1[0].y)));
        }
        return hit;
    }

    polyline_t get_polyline(str_view line)
    {
        polyline_t out = { vec2i{0, 0} };
        split_multi(line, ',', [&](str_view part)
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
    size_t get_dist(const vec2i& in) { return abs(in.x) + abs(in.y); }

    // get manhattan distance between two vectors
    size_t get_dist(const vec2i& a, const vec2i& b) { return abs(a.x - b.x) + abs(a.y - b.y); }

    // get manhattan distance for a segment
    size_t get_dist(const segment_t& s) { return abs(s[0].x - s[1].x) + abs(s[0].y - s[1].y);}

    cross_list_t get_intersections(const polyline_t& a, const polyline_t& b)
    {
        cross_list_t out;
        size_t dist_a = 0;
        for (size_t idx_a = 0; idx_a < a.size() - 1; ++idx_a)
        {
            segment_t sa { a[idx_a], a[idx_a+1] };
            dist_a += get_dist(sa);

            size_t dist_b = 0;
            for (size_t idx_b = 0; idx_b < b.size() - 1; ++idx_b)
            {
                segment_t sb { b[idx_b], b[idx_b+1] };
                dist_b += get_dist(sb);

                vec2i point;
                if (intersect(sa, sb, point))
                {
                    size_t extra_dist = get_dist(sa[1], point) + get_dist(sb[1], point);
                    out.push_back({ point, dist_a + dist_b - extra_dist });
                }
            }
        }

        return out;
    }

    vec2i get_closest_pt1(const cross_list_t& points)
    {
        vec2i closest_point { INT_MAX / 2, INT_MAX / 2 };

        for (const auto& [point, dist]: points)
            if (point != vec2i{0, 0} && get_dist(point) < get_dist(closest_point))
                closest_point = point;

        return closest_point;
    }

    size_t get_distance_pt2(const cross_list_t& points)
    {
        size_t closest_dist = SIZE_MAX;

        for (const auto& [point, dist]: points)
            if (point != vec2i{0, 0} && dist < closest_dist)
                closest_dist = dist;

        return closest_dist;
    }
}

output_t Day_2019_3::run_solution(str_view input) const
{
    std::vector<polyline_t> lines;
    parse_input(input, [&](str_view line)
    {
        lines.push_back(get_polyline(line));
    });

    const auto& points = get_intersections(lines[0], lines[1]);
    size_t part1 = get_dist(get_closest_pt1(points));
    size_t part2 = get_distance_pt2(points);

    return {part1, part2};
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
        auto line1 = get_polyline(str_view{ inputs[0].data(), inputs[0].length() });
        auto line2 = get_polyline(str_view{ inputs[1].data(), inputs[1].length() });
        auto points = get_intersections(line1, line2);
        assert(get_dist(get_closest_pt1(points)) == 6);
        assert(get_distance_pt2(points) == 30);
    }

    {
        auto line1 = get_polyline(str_view{ inputs[2].data(), inputs[2].length() });
        auto line2 = get_polyline(str_view{ inputs[3].data(), inputs[3].length() });
        auto points = get_intersections(line1, line2);
        assert(get_dist(get_closest_pt1(points)) == 159);
        assert(get_distance_pt2(points) == 610);
    }

    {
        auto line1 = get_polyline(str_view{ inputs[4].data(), inputs[4].length() });
        auto line2 = get_polyline(str_view{ inputs[5].data(), inputs[5].length() });
        auto points = get_intersections(line1, line2);
        assert(get_dist(get_closest_pt1(points)) == 135);
        assert(get_distance_pt2(points) == 410);
    }
}
