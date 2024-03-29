#include "setup/all.h"
#include "intcode.h"

ADVENT_DAY(2019, 2, 4090689, 7733);

namespace
{
    int64_t run_single_guess(intcode_solver_t& solver, uint8_t verb, uint8_t noun)
    {
        solver.reset();
        solver.memory[1] = verb;
        solver.memory[2] = noun;
        solver.execute(false);

        return solver.memory[0];
    }

    int64_t run_multiple_guesses(intcode_solver_t& solver, int64_t expected)
    {
        for (uint8_t i = 0; i < 99; ++i)
            for (uint8_t j = 0; j < 99; ++j)
                if (expected == run_single_guess(solver, i, j))
                    return 100 * i + j;

        assert(false);
        return 0;
    }
}

result_t Day_2019_2::run_solution(str_view input) const
{
    intcode_solver_t solver(input);

    size_t part1 = run_single_guess(solver, 12, 2);
    size_t part2 = run_multiple_guesses(solver, 19690720);

    return {part1, part2};
}

void Day_2019_2::run_tests() const
{
    intcode_solver_t::run_tests();
}
