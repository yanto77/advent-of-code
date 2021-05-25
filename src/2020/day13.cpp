#include "advent2020.h"

ADVENT_DAY(2020, 13, 2947, 526090562196173);

struct data_t
{
    int t0 = 0;
    std::vector<vec2i> buslines = {};
};

namespace
{
    vec2i intersect(const vec2i& lhs, const vec2i& rhs)
    {
        auto& [a, c] = lhs;
        auto& [b, d] = rhs;

        double ab = a - b;
        double dc = d - c;
        if (fabs(ab) < __DBL_EPSILON__)
            assert(false); // parallel lines

        int x = static_cast<int>(ceil(dc / ab));
        int y = b * x + d;

        return { x, y };
    }

    std::vector<vec2i> parse_eq_line(const sv& line)
    {
        std::vector<vec2i> eqs;

        eqs.reserve(100);
        const auto& parts = split_multi(line, ',');
        for (size_t i = 0; i < parts.size(); ++i)
        {
            if (parts[i][0] != 'x')
            {
                int m = to_int<int>(parts[i]);
                int r = static_cast<int>(i);
                eqs.push_back(vec2i { (m - r) % m, m });
            }
        }

        return eqs;
    }

    data_t parse_input(const input_t& input)
    {
        data_t parsed;

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
                parsed.buslines = parse_eq_line(line);
            }
        });

        return parsed;
    }

    std::pair<int, int> evaluate_pt1(const data_t& data)
    {
        // Consider input vec2i's to contain {m, b} pairs for
        // a system of linear equations: y = (m)x + (b),
        const vec2i base { 0, data.t0 };
        const size_t n = data.buslines.size();

        // Intersect all buslines with expected base-line
        std::vector<int> result(n, 0);
        for (size_t i = 0; i < n; ++i)
        {
            const vec2i& point = intersect(base, { data.buslines[i].y, 0 });
            result[i] = point.y;
        }

        // Find result with smallest value
        std::pair<int, int> min { INT32_MAX, 0 }; // { value, bus ID }
        for (int i = 0; i < n; ++i)
            if (data.t0 < result[i] && result[i] < min.first)
                min = { result[i], i };

        return { (min.first - data.t0), data.buslines[min.second].y };
    }

    /// Consider input vec2i's to contain {a, m} pairs for
    /// a system of linear congruences: x = a (mod m)
    /// Solves for x.
    int64_t solve_congruence_sys(const std::vector<vec2i>& eqs)
    {
        // Ref: https://www.dave4math.com/mathematics/chinese-remainder-theorem/
        // Ref: https://fasterthanli.me/series/advent-of-code-2020/part-13
        int64_t res = 0;

        int64_t N = 1;
        for (const vec2i& eq : eqs)
        {
            N *= eq.y;
        }

        for (const vec2i& eq : eqs)
        {
            const auto& [a_i, n_i] = eq;
            int64_t nh_i = N / n_i;

            // Solve u_i (Modular_multiplicative_inverse) using Euler's theorem
            // Ref: https://en.wikipedia.org/wiki/Modular_multiplicative_inverse#Using_Euler's_theorem
            int64_t u_i = binpow(nh_i, n_i - 2, n_i) % (n_i);

            // Apply
            res += (a_i * nh_i * u_i);
            res %= N;
        }

        if (res < 0)
            res += N;

        return res;
    }
}

output_t Day_2020_13::run_solution(const input_t& input) const
{
    data_t data = parse_input(input);

    const auto& [diff, bus] = evaluate_pt1(data);
    const size_t part1 = diff * bus;
    const size_t part2 = solve_congruence_sys(data.buslines);
    return { part1, part2 };
}

void Day_2020_13::run_tests() const
{
    // Part 1
    {
        char text[] = "939\n7,13,x,x,59,x,31,19\n";
        input_t test1 { text, sizeof(text) };

        data_t data = parse_input(test1);
        assert(data.t0 == 939);
        assert(data.buslines == std::vector<vec2i>({
            vec2i {0, 7},
            vec2i {12, 13},
            vec2i {55, 59},
            vec2i {25, 31},
            vec2i {12, 19}
        }));

        const auto& [diff, bus] = evaluate_pt1(data);
        assert(diff == 5 && bus == 59);
    }

    // Part 2
    {
        assert(3417 == solve_congruence_sys(parse_eq_line("17,x,13,19")));
        assert(754018 == solve_congruence_sys(parse_eq_line("67,7,59,61")));
        assert(779210 == solve_congruence_sys(parse_eq_line("67,x,7,59,61")));
        assert(1261476 == solve_congruence_sys(parse_eq_line("67,7,x,59,61")));
        assert(1202161486 == solve_congruence_sys(parse_eq_line("1789,37,47,1889")));
    }
}
