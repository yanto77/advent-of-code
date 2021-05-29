#include "intcode.h"
#include "setup/all.h"
#include <thread>

namespace
{
    constexpr bool DEBUG_PRINT = false;

    struct intcode_test
    {
        std::string input;
        std::vector<int32_t> end_state;
    };

    const std::vector<intcode_test> test_data =
    {
        intcode_test {
            .input = "1,0,0,0,99\n",
            .end_state = { 2, 0, 0, 0, 99 }
        },
        intcode_test {
            .input = "2,3,0,3,99\n",
            .end_state = { 2, 3, 0, 6, 99 }
        },
        intcode_test {
            .input = "2,4,4,5,99,0\n",
            .end_state = { 2, 4, 4, 5, 99, 9801 }
        },
        intcode_test {
            .input = "1,1,1,4,99,5,6,0,99\n",
            .end_state = { 30, 1, 1, 4, 2, 5, 6, 0, 99 }
        },
        intcode_test {
            .input = "1,9,10,3,2,3,11,0,99,30,40,50\n",
            .end_state = { 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50 }
        },
    };

    const char* to_str(uint8_t value)
    {
        switch (value)
        {
            case SUM: return "SUM";
            case MULTIPLY: return "MULTIPLY";
            case INPUT: return "INPUT";
            case OUTPUT: return "OUTPUT";
            case HALT: return "HALT";
            default: return "<ERR>";
        }
    }
}

void intcode_solver_t::run_tests()
{
    for (const auto& test: test_data)
    {
        intcode_solver_t solver(test.input);
        // printf("Input : "); print_row(data);

        solver.execute();
        // printf("Result: "); print_row(data);

        assert(solver.program == test.end_state);
    }
}

void intcode_solver_t::debug_pre_exec()
{
    if (!DEBUG_PRINT)
        return;

    const auto& [opcode, m1, m2, m3] = get_opcode();

    fmt::print("opcode: {} (instr: {}, ip {})\n", opcode, to_str(opcode), ip);
    fmt::print("  pre memory : ");
    for (uint8_t i = 0; i < 8; ++i)
    {
        fmt::print("{:4}, ", program[ip + i]);
    }
    fmt::print("\n");
}

void intcode_solver_t::debug_post_exec()
{
    if (!DEBUG_PRINT)
        return;

    fmt::print("  post memory: ");
    for (uint8_t i = 0; i < 8; ++i)
    {
        fmt::print("{:4}, ", program[ip + i]);
    }
    fmt::print("\n");

    fmt::print("  output     : "); print_row(output_data); fmt::print("\n");

    fmt::print("\n");
    std::this_thread::sleep_for(100ms);
}
