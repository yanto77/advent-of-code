#include "setup/all.h"
#include "intcode.h"

ADVENT_DAY(2019, 2, 4090689, 7733);

namespace
{
    int32_t run_single_guess(intcode_solver_t solver, uint8_t verb, uint8_t noun)
    {
        solver.program[1] = verb;
        solver.program[2] = noun;
        solver.execute();
        return solver.program[0];
    }

    int32_t run_multiple_guesses(intcode_solver_t solver, int32_t expected)
    {
        for (uint8_t i = 0; i < 99; ++i)
            for (uint8_t j = 0; j < 99; ++j)
                if (expected == run_single_guess(solver, i, j))
                    return 100 * i + j;

        assert(false);
        return 0;
    }
}

output_t Day_2019_2::run_solution(const input_t& input) const
{
    intcode_solver_t solver(get_sv(input));

    size_t part1 = run_single_guess(solver, 12, 2);
    size_t part2 = run_multiple_guesses(solver, 19690720);

    return {part1, part2};
}

void Day_2019_2::run_tests() const
{
    intcode_solver_t::run_tests();
}
