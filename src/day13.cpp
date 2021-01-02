#include "advent2020.h"

/// line equation: y = mx + b
struct line_t
{
    int m = 0;
    int b = 0;

    friend bool operator==(const line_t& lhs, const line_t& rhs)
    {
        return lhs.m == rhs.m && lhs.b == rhs.b;
    }

    static vec2i intersect(const line_t& lhs, const line_t& rhs)
    {
        auto& [a, c] = lhs;
        auto& [b, d] = rhs;

        double ab = a - b;
        double dc = d - c;
        if (fabs(ab) < __DBL_EPSILON__) assert(false); // parallel lines

        int x = static_cast<int>(ceil(dc / ab));
        int y = b * x + d;

        return {x, y};
    }
};

struct data_t
{
    int t0 = 0;
    std::vector<line_t> buslines = {};
};

namespace
{
    data_t parse_input(const input_t& input)
    {
        data_t parsed;
        parsed.buslines.reserve(100);

        bool first = true;
        parse_input(input, [&](const sv& line)
        {
            if (first)
            {
                parsed.t0 = to_int<int>(line);
                first = false;
            }
            else
            {
                const auto& parts = split_multi(line, ',');
                for (size_t i = 0; i < parts.size(); ++i)
                {
                    if (parts[i][0] != 'x')
                    {
                        parsed.buslines.push_back(line_t {
                            .m = to_int<int>(parts[i]),
                            .b = -1 * static_cast<int>(i)
                        });
                    }
                }
            }
        });

        return parsed;
    }

    std::pair<int, int> evaluate_pt1(const data_t& data)
    {
        const line_t base{0, data.t0};
        const int n = static_cast<int>(data.buslines.size());

        // Intersect all buslines with expected base-line
        std::vector<int> result { n, 0 };
        for (size_t i = 0; i < n; ++i)
        {
            const vec2i& point = line_t::intersect(base, { data.buslines[i].m, 0 });
            result[i] = point.y;
        }

        // Find result with smallest value
        std::pair<int, int> min { INT32_MAX, 0 }; // { value, bus ID }
        for (int i = 0; i < n; ++i)
            if (data.t0 < result[i] && result[i] < min.first)
                min = { result[i], i };

        return {(min.first - data.t0), data.buslines[min.second].m};
    }
}

output_t day13(const input_t& input)
{
    data_t data = parse_input(input);

    const auto& [diff, bus] = evaluate_pt1(data);
    printf("diff: %d, bus: %d, result: %d\n", diff, bus, diff*bus);

    return { static_cast<size_t>(diff*bus), 0 };
}

void day13_test()
{
    char text[25] = "939\n7,13,x,x,59,x,31,19\n";
    input_t test1 { text, 25 };

    data_t data = parse_input(test1);
    assert(data.t0 == 939);
    assert(data.buslines == std::vector<line_t>({
        line_t {7, 0},
        line_t {13, -1},
        line_t {59, -4},
        line_t {31, -6},
        line_t {19, -7}
    }));

    const auto& [diff, bus] = evaluate_pt1(data);
    assert(diff == 5 && bus == 59);
}

/*
find n, where
    n = (b[0] * k1) + 0 =  7 * k1 + 0
    n = (b[1] * k2) + 1 = 13 * k2 + 1
    n = (b[2] * k3) + 4 = 59 * k3 + 4
    n = (b[3] * k4) + 6 = 31 * k4 + 6
    n = (b[4] * k5) + 7 = 19 * k5 + 7

1068781
    7 * 152683 - 0  = 1068781
    13 * 82214 - 1  = 1068781
    59 * 18115 - 4  = 1068781
    31 * 34477 - 6  = 1068781
    19 * 56252 - 7  = 1068781

x = 1068781, not really right, is it?
    x mod 7 = 0
    x mod 13 = 1
    x mod 59 = 4
    x mod 31 = 6
    x mod 19 = 7

    7 * 152683 - 0  = 1068781
    13 * 82214 - 1  = 1068781
    59 * 18115 - 4  = 1068781
    31 * 34477 - 6  = 1068781
    19 * 56252 - 7  = 1068781

see also:
* https://sites.millersville.edu/bikenaga/number-theory/systems-of-congruences/systems-of-congruences.html
* https://www.reddit.com/r/adventofcode/comments/kc4njx/2020_day_13_solutions/ggv5a0s/?utm_source=reddit&utm_medium=web2x&context=3
    > I found out that the "Chinese remainder theorem" was involved so I used
    > the following videos to gain a grasp of how modular arithmetic and the theorem worked:
    >    https://youtu.be/5OjZWSdxlU0
    >    https://youtu.be/6dZLq77gSGU
    >    https://youtu.be/zIFehsBHB8o
    > also:
    > https://www.dcode.fr/chinese-remainder

* https://github.com/blu3r4y/AdventOfLanguages2020/blob/main/src/day13.cpp

*/
