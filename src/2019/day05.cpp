#include "setup/all.h"
#include "intcode.h"

ADVENT_DAY(2019, 5, 4511442, 0);

namespace
{
}

output_t Day_2019_5::run_solution(const input_t& input) const
{
    intcode_solver_t solver(get_sv(input));
    const auto& output = solver.execute({ 1 });

    return {static_cast<size_t>(output.back()), 0};
}

void Day_2019_5::run_tests() const
{
    // intcode_solver_t solver(std::string("1,0,3,3,1005,2,10,5,1,0,4,1,99"));
    // auto output = solver.execute({});
    // printf("\n\n => final output: "); print_row(output);
}
