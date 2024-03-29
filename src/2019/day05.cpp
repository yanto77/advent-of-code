#include "setup/all.h"
#include "intcode.h"

ADVENT_DAY(2019, 5, 4511442, 12648139);

result_t Day_2019_5::run_solution(str_view input) const
{
    intcode_solver_t solver(input);

    solver.reset();
    solver.execute(false, { 1 });
    size_t part1 = solver.output_data.back();

    solver.reset();
    solver.execute(false, { 5 });
    size_t part2 = solver.output_data.back();

    return {part1, part2};
}

void Day_2019_5::run_tests() const
{
}
