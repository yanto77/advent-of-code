#include "advent2020.h"

namespace
{
    enum class op_t { ADD, MULT };
    struct stack_t { int64_t num; op_t op; };

    [[maybe_unused]]
    void print(const std::vector<stack_t>& stacks2)
    {
        printf("Stacks: ");
        for (auto& stack: stacks2)
        {
            printf("[num: %ld, op: %c], ", stack.num, stack.op == op_t::ADD ? '+' : '*');
        }
        printf("\n");
    }

    int64_t solve_single(const sv& line)
    {
        // std::cout << line << std::endl;
        constexpr stack_t empty_stack = { 0, op_t::ADD };
        std::vector<stack_t> stacks { empty_stack };

        auto apply = [](stack_t& to, int64_t num)
        {
            if (to.op == op_t::ADD)
            {
                // printf("applying: %d + %d => %d\n", to.num, num, to.num + num);
                to.num += num;
            }
            else if (to.op == op_t::MULT)
            {
                // printf("applying: %d * %d => %d\n", to.num, num, to.num * num);
                to.num *= num;
            }
        };

        for (size_t n = 0; n < line.size(); ++n)
        {
            switch (line[n])
            {
                case ' ': break;
                case '\n': break;
                case '-': assert(false); break;
                case '+': stacks.back().op = op_t::ADD; break;
                case '*': stacks.back().op = op_t::MULT; break;
                case '(':
                {
                    // printf("=> push stack\n");
                    stacks.push_back(empty_stack);
                    break;
                }
                case ')':
                {
                    // printf("=> pop stack, applying %d\n", stacks.back().num);
                    auto& prev = (stacks[stacks.size() - 2]);
                    apply(prev, stacks.back().num);
                    stacks.pop_back();
                    break;
                }
                default:
                {
                    size_t end = line.find_first_not_of("0123456789", n);
                    sv token { &line[n], end - n };
                    apply(stacks.back(), to_int<uint8_t>(token));
                    n = end - 1;
                }
            }
        }

        return stacks.back().num;
    }
}

output_t day18(const input_t& input)
{
    int64_t part1 = 0;
    parse_input(input, [&](const sv& line)
    {
        int64_t value = solve_single(line);
        part1 += value;
        printf("value: %ld\n", value);

    });
    printf("part1: %ld\n", part1);

    // assert(part1 != 10684594594);
    // assert(part1 > 2094660002);
    assert(part1 == 4696493914530);
    return { 0, 0 };
}

void day18_test()
{
    assert(solve_single("1 + 2 * 3 + 4 * 5 + 6\n") == 71);
    assert(solve_single("1 + (2 * 3) + (4 * (5 + 6))\n") == 51);
    assert(solve_single("2 * 3 + (4 * 5)\n") == 26);
    assert(solve_single("5 + (8 * 3 + 9 + 3 * 4 * 3)\n") == 437);
    assert(solve_single("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))\n") == 12240);
    assert(solve_single("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2\n") == 13632);
}
