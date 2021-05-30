#include "setup/all.h"
#include "intcode.h"
#include <fmt/ranges.h>

ADVENT_DAY(2019, 7, 38500, 33660560);

namespace
{
    typedef std::function<size_t(intcode_solver_t&, const std::array<uint8_t, 5>&)> test_config_fn;
    constexpr std::array<uint8_t, 5> phases_pt1 = {0, 1, 2, 3, 4};
    constexpr std::array<uint8_t, 5> phases_pt2 = {5, 6, 7, 8, 9};

    size_t test_config_pt1(intcode_solver_t& base, const std::array<uint8_t, 5>& phases)
    {
        int32_t signal = 0;
        for (uint8_t idx = 0; idx < 5; ++idx)
        {
            base.reset();
            base.io.set_input({phases[idx], signal});
            base.execute();
            signal = base.io.get_output().front();
        }
        return signal;
    }

    size_t test_config_pt2(intcode_solver_t& base, const std::array<uint8_t, 5>& phases)
    {
        std::array<intcode_solver_t, 5> amps {base, base, base, base, base};

        for (uint8_t idx = 0; idx < 5; ++idx)
        {
            amps[idx].reset();
            amps[idx].io.set_input({ phases[idx] });
        }

        int32_t signal = 0;
        while (std::all_of(amps.begin(), amps.end(), [](const auto& s) { return !s.is_halted; }))
        {
            for (uint8_t idx = 0; idx < 5; ++idx)
            {
                amps[idx].io.append_input(signal);
                amps[idx].execute(true);
                signal = amps[idx].io.get_output().back();
            }
        }
        return signal;
    }

    size_t find_max_signal(intcode_solver_t solver, std::array<uint8_t, 5> phases, test_config_fn test_fn)
    {
        size_t max_signal = 0;
        do 
        {
            max_signal = max(max_signal, test_fn(solver, phases));
        } while (std::next_permutation(phases.begin(), phases.end()));
        return max_signal;
    }
}

output_t Day_2019_7::run_solution(const input_t& input) const
{
    intcode_solver_t solver(get_sv(input));
    size_t part1 = find_max_signal(solver, phases_pt1, test_config_pt1);
    size_t part2 = find_max_signal(solver, phases_pt2, test_config_pt2);

    return {part1, part2};
}

void Day_2019_7::run_tests() const
{
    {
        intcode_solver_t solver(std::string {
            "3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0"
        });
        assert(find_max_signal(solver, phases_pt1, test_config_pt1) == 43210);
    }
    {
        intcode_solver_t solver(std::string {
            "3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,"
            "23,1,24,23,23,4,23,99,0,0"
        });
        assert(find_max_signal(solver, phases_pt1, test_config_pt1) == 54321);
    }
    {
        intcode_solver_t solver(std::string {
            "3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,"
            "33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,"
            "0,0"
        });
        assert(find_max_signal(solver, phases_pt1, test_config_pt1) == 65210);
    }
    {
        intcode_solver_t solver(std::string {
            "3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,"
            "1005,55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,"
            "54,0,55,1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,"
            "1005,56,6,99,0,0,0,0,10"
        });
        assert(find_max_signal(solver, phases_pt2, test_config_pt2) == 18216);
    }
    {
        intcode_solver_t solver(std::string {
            "3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,"
            "27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5"
        });
        assert(find_max_signal(solver, phases_pt2, test_config_pt2) == 139629729);
    }
}
