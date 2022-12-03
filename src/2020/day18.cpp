#include "setup/all.h"
#include <stack>

ADVENT_DAY(2020, 18, 4696493914530, 362880372308125);

namespace
{
    typedef std::map<char, int> prec_map_t;

    const prec_map_t pt1 = { { '+', 1 }, { '*', 1 } };
    const prec_map_t pt2 = { { '+', 2 }, { '*', 1 } };

    size_t solve_single(const prec_map_t& prec, str_view line)
    {
        std::stack<size_t> nums;
        std::stack<char> ops;

        auto apply = [&nums, &ops](char op)
        {
            ops.pop();

            size_t n1 = nums.top();
            nums.pop();

            if (op == '*') nums.top() *= n1;
            else if (op == '+') nums.top() += n1;
        };

        for (size_t n = 0; n < line.size(); ++n)
        {
            switch (line[n])
            {
                case ' ': break;
                case '\n': break;
                case '(':
                {
                    ops.push(line[n]);
                    break;
                }
                case ')':
                {
                    while (!ops.empty())
                    {
                        if (ops.top() != '(')
                        {
                            apply(ops.top());
                        }
                        else
                        {
                            ops.pop();
                            break;
                        }
                    }
                    break;
                }
                case '+':
                case '*':
                case '-':
                {
                    char op = line[n];
                    while (!ops.empty())
                    {
                        char top_op = ops.top();
                        if (top_op != '(' && prec.at(top_op) >= prec.at(op))
                        {
                            apply(top_op);
                        }
                        else
                            break;
                    }
                    ops.push(op);
                    break;
                }
                default: // numbers
                {
                    const auto& [num, end] = to_int<size_t>(line, n);
                    nums.push(num);
                    n = end - 1;
                }
            }
        }

        while (!ops.empty())
            apply(ops.top());

        return nums.top();
    }
}

result_t Day_2020_18::run_solution(str_view input) const
{
    size_t part1 = 0;
    size_t part2 = 0;
    for_each_split(input, '\n', [&](str_view line)
    {
        part1 += solve_single(pt1, line);
        part2 += solve_single(pt2, line);
    });
    return { part1, part2 };
}

void Day_2020_18::run_tests() const
{
    assert(solve_single(pt1, "1 + 2 * 3 + 4 * 5 + 6\n") == 71);
    assert(solve_single(pt1, "1 + (2 * 3) + (4 * (5 + 6))\n") == 51);
    assert(solve_single(pt1, "2 * 3 + (4 * 5)\n") == 26);
    assert(solve_single(pt1, "5 + (8 * 3 + 9 + 3 * 4 * 3)\n") == 437);
    assert(solve_single(pt1, "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))\n") == 12240);
    assert(solve_single(pt1, "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2\n") == 13632);

    assert(solve_single(pt2, "1 + 2 * 3 + 4 * 5 + 6\n") == 231);
    assert(solve_single(pt2, "1 + (2 * 3) + (4 * (5 + 6))\n") == 51);
    assert(solve_single(pt2, "2 * 3 + (4 * 5)\n") == 46);
    assert(solve_single(pt2, "5 + (8 * 3 + 9 + 3 * 4 * 3)\n") == 1445);
    assert(solve_single(pt2, "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))\n") == 669060);
    assert(solve_single(pt2, "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2\n") == 23340);
}
