#include "setup/all.h"
#include "intcode.h"

ADVENT_DAY(2019, 9, 3507134798, 84513);

output_t Day_2019_9::run_solution(const input_t& input) const
{
    intcode_solver_t solver(get_sv(input));
    solver.execute(false, { 1 });
    size_t part1 = solver.output_data.front();
    
    solver.reset();
    solver.execute(false, { 2 });
    size_t part2 = solver.output_data.front();

    return {part1, part2};
}

void Day_2019_9::run_tests() const
{
    // 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 takes no input and produces a copy of itself as output.
    //  should output a 16-digit number.
    // should output the large number in the middle.

    {
        intcode_solver_t solver(std::string {
            "109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99"
        });
        solver.execute(false);
        std::vector<int64_t> expected = {
            109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99
        };
        assert(solver.output_data == expected);
    }
    {
        intcode_solver_t solver(std::string {"1102,34915192,34915192,7,4,7,99,0"});
        solver.execute(false);
        std::vector<int64_t> expected = {1219070632396864};
        assert(solver.output_data == expected);
    }
    {
        intcode_solver_t solver(std::string {"104,1125899906842624,99"});
        solver.execute(false);
        std::vector<int64_t> expected = {1125899906842624};
        assert(solver.output_data == expected);
    }

    // TODO:
    // 1) rel params
    // 2) The computer's available memory should be much larger than the initial program. Memory beyond the initial program starts with the value 0 and can be read or written like any other memory. (It is invalid to try to access memory at a negative address, though.)
}
