#include "setup/all.h"
#include "intcode.h"

ADVENT_DAY(2019, 5, 4511442, 12648139);

output_t Day_2019_5::run_solution(const input_t& input) const
{
    intcode_solver_t solver(get_sv(input));
    size_t part1 = solver.execute({ 1 }).back();
    solver.reset();

    size_t part2 = solver.execute({ 5 }).back();

    return {part1, part2};
}

void Day_2019_5::run_tests() const
{
}
