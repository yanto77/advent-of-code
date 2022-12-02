#include "setup/all.h"

ADVENT_DAY(2021, 10, 321237, 2360030859);

namespace
{
    int get_score(char value)
    {   
        // Part 1
        if (value == ')') return 3;
        if (value == ']') return 57;
        if (value == '}') return 1197;
        if (value == '>') return 25137;
        // Part 2
        if (value == '(') return 1;
        if (value == '[') return 2;
        if (value == '{') return 3;
        if (value == '<') return 4;

        assert(false);
        return 0;
    }

    bool is_pair(char lhs, char rhs)
    {
        if (lhs == '(' && rhs == ')') return true;
        if (lhs == '[' && rhs == ']') return true;
        if (lhs == '{' && rhs == '}') return true;
        if (lhs == '<' && rhs == '>') return true;
        return false;
    }

    std::pair<size_t, size_t> solve(str_view input)
    {
        size_t part1 = 0;
        std::vector<size_t> part2vec {};

        parse_input(input, [&](str_view line){

            std::stack<char> symbols;
            bool valid = true;

            for (char ch: line)
            {
                if (ch == '[' || ch == '(' || ch == '{' || ch == '<')
                {
                    symbols.push(ch);
                }
                else if (is_pair(symbols.top(), ch))
                {
                    symbols.pop();
                }
                else
                {
                    valid = false;
                    part1 += get_score(ch);
                    break;
                }
            }

            if (valid)
            {
                size_t tmp = 0;
                while (!symbols.empty())
                {
                    tmp = 5 * tmp + get_score(symbols.top());
                    symbols.pop();
                }

                part2vec.push_back(tmp);
            }
        });

        std::sort(part2vec.begin(), part2vec.end());
        size_t part2 = part2vec[(part2vec.size() / 2)];
        return { part1, part2 };
    }
}

result_t Day_2021_10::run_solution(str_view input) const
{
    const auto& [part1, part2] = solve(input);
    return { part1, part2 };
}

void Day_2021_10::run_tests() const
{
    char text1[] =
        "[({(<(())[]>[[{[]{<()<>>\n"
        "[(()[<>])]({[<{<<[]>>(\n"
        "{([(<{}[<>[]}>{[]{[(<()>\n"
        "(((({<>}<{<{<>}{[]{[]{}\n"
        "[[<[([]))<([[{}[[()]]]\n"
        "[{[{({}]{}}([{[{{{}}([]\n"
        "{<[[]]>}<{[{[{[]{()[[[]\n"
        "[<(<(<(<{}))><([]([]()\n"
        "<{([([[(<>()){}]>(<<{{\n"
        "<{([{{}}[<[[[<>{}]]]>[]]\n"
        "";

    const auto& [part1, part2] = solve(text1);
    assert(part1 == 26397);
    assert(part2 == 288957);
}
