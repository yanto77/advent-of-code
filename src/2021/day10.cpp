#include "setup/all.h"

ADVENT_DAY(2021, 10, 321237, 2360030859);

namespace
{
    const std::unordered_map<char, int> pt1_scores = {
        { ')', 3 },
        { ']', 57 },
        { '}', 1197 },
        { '>', 25137 },
    };
    const std::unordered_map<char, int> pt2_scores = {
        { '(', 1 },
        { '[', 2 },
        { '{', 3 },
        { '<', 4 },
    };
    const std::unordered_map<char, char> symbol_pairs = {
        { '(', ')' },
        { '[', ']' },
        { '{', '}' },
        { '<', '>' },
    };

    std::pair<size_t, size_t> solve(const input_t& input)
    {
        size_t part1 = 0;
        std::vector<size_t> part2vec {};

        parse_input(input, [&](const sv& line){

            std::stack<char> symbols;
            bool valid = true;

            for (char ch: line)
            {
                if (ch == '[' || ch == '(' || ch == '{' || ch == '<')
                {
                    symbols.push(ch);
                }
                else if (symbol_pairs.at(symbols.top()) == ch)
                {
                    symbols.pop();
                }
                else
                {
                    valid = false;
                    part1 += pt1_scores.at(ch);
                    break;
                }
            }

            if (valid)
            {
                size_t tmp = 0;
                while (!symbols.empty())
                {
                    tmp = 5 * tmp + pt2_scores.at(symbols.top());
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

output_t Day_2021_10::run_solution(const input_t& input) const
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
    input_t test1 { text1, sizeof(text1) };

    const auto& [part1, part2] = solve(test1);
    assert(part1 == 26397);
    assert(part2 == 288957);
}
