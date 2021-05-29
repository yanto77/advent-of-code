#include "setup/all.h"
#include "intcode.h"
#include <fmt/ranges.h>

ADVENT_DAY(2019, 7, 38500, 0);

namespace
{
    /// returns output signal from specific arrangement of amplifier phases
    size_t test_config(intcode_solver_t& solver, const std::array<uint8_t, 5>& phases)
    {
        int32_t signal = 0;
        for (uint8_t idx = 0; idx < 5; ++idx)
        {
            solver.reset();
            signal = solver.execute({phases[idx], signal}).front();
        }
        return signal;
    }

    size_t find_max_signal(intcode_solver_t& solver)
    {
        size_t max_signal = 0;

        std::array<uint8_t, 5> phases {0, 1, 2, 3, 4};
        do 
        {
            max_signal = max(max_signal, test_config(solver, phases));
        } while (std::next_permutation(phases.begin(), phases.end()));

        return max_signal;
    }
}

output_t Day_2019_7::run_solution(const input_t& input) const
{
    intcode_solver_t solver(get_sv(input));
    size_t part1 = find_max_signal(solver);

    return {part1, 0};
}

void Day_2019_7::run_tests() const
{
    {
        intcode_solver_t solver(std::string {"3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0"});
        assert(find_max_signal(solver) == 43210);
    }
    {
        intcode_solver_t solver(std::string {"3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0"});
        assert(find_max_signal(solver) == 54321);
    }
    {
        intcode_solver_t solver(std::string {"3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0"});
        assert(find_max_signal(solver) == 65210);
    }
}
