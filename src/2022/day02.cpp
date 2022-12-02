#include "setup/all.h"

ADVENT_DAY(2022, 2, 11666, 12767);

namespace 
{
    enum Hand { Rock = 0, Paper = 1, Scissors = 2 };
    enum Result { Lose = 0, Draw = 1, Win = 2 };
    struct Round { 
        Hand opponent; 
        union { Hand you; Result result; };
    };

    // Combined scores for single round:
    //   - the shape you selected (Rock=1, Paper=2, Scissors=3) 
    //   - the outcome of the round (lost=0, draw=3, won=6).
    static const uint8_t s_points[9] = {
        (1+3), (2+6), (3+0),
        (1+0), (2+3), (3+6),
        (1+6), (2+0), (3+3)
    };

    // Strategy based on opponents hand (horizontal) + intended result (vertical).
    static const Hand s_strategies[9] = {
        (Hand::Scissors), (Hand::Rock),     (Hand::Paper),
        (Hand::Rock),     (Hand::Paper),    (Hand::Scissors),
        (Hand::Paper),    (Hand::Scissors), (Hand::Rock)
    };

    Round parse_line(str_view line)
    {
        Round round;

        if (line[0] == 'A') round.opponent = Hand::Rock;
        if (line[0] == 'B') round.opponent = Hand::Paper;
        if (line[0] == 'C') round.opponent = Hand::Scissors;

        if (line[2] == 'X') round.you = Hand::Rock;
        if (line[2] == 'Y') round.you = Hand::Paper;
        if (line[2] == 'Z') round.you = Hand::Scissors;

        return round;
    }
}

result_t Day_2022_2::run_solution(str_view input) const
{
    size_t part1 = 0;
    size_t part2 = 0;

    parse_input(input, [&](str_view line)
    {
        Round round = parse_line(line);
        part1 += s_points[(round.opponent * 3) + round.you];

        Hand pt2_strategy = s_strategies[(round.opponent * 3) + round.result];
        part2 += s_points[(round.opponent * 3) + pt2_strategy];
    });

    return { part1, part2 };
}

void Day_2022_2::run_tests() const
{
}
