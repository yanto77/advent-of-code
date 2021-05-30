#include "intcode.h"
#include "setup/all.h"
#include <thread>

namespace
{
    constexpr bool DEBUG_PRINT = false;

    struct intcode_test
    {
        std::string program;
        std::vector<int64_t> end_state;
        std::vector<int64_t> input;
        std::vector<int64_t> output;
    };

    const std::vector<intcode_test> test_data =
    {
        //
        // Day 02
        //

        intcode_test {
            .program = "1,0,0,0,99\n",
            .end_state = { 2, 0, 0, 0, 99 },
            .input = {},
            .output = {}
        },
        intcode_test {
            .program = "2,3,0,3,99\n",
            .end_state = { 2, 3, 0, 6, 99 },
            .input = {},
            .output = {}
        },
        intcode_test {
            .program = "2,4,4,5,99,0\n",
            .end_state = { 2, 4, 4, 5, 99, 9801 },
            .input = {},
            .output = {}
        },
        intcode_test {
            .program = "1,1,1,4,99,5,6,0,99\n",
            .end_state = { 30, 1, 1, 4, 2, 5, 6, 0, 99 },
            .input = {},
            .output = {}
        },
        intcode_test {
            .program = "1,9,10,3,2,3,11,0,99,30,40,50\n",
            .end_state = { 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50 },
            .input = {},
            .output = {}
        },

        //
        // Day 05
        //

        // test if input is equal to 8, using position mode
        intcode_test {
            .program = "3,9,8,9,10,9,4,9,99,-1,8\n",
            .end_state = { 3, 9, 8, 9, 10, 9, 4, 9, 99, 1, 8 },
            .input = { 8 },
            .output = { 1 }
        },
        intcode_test {
            .program = "3,9,8,9,10,9,4,9,99,-1,8\n",
            .end_state = { 3, 9, 8, 9, 10, 9, 4, 9, 99, 0, 8 },
            .input = { 10 },
            .output = { 0 }
        },

        // test if input is less than 8, using position mode
        intcode_test {
            .program = "3,9,7,9,10,9,4,9,99,-1,8\n",
            .end_state = { 3, 9, 7, 9, 10, 9, 4, 9, 99, 1, 8 },
            .input = { 6 },
            .output = { 1 }
        },
        intcode_test {
            .program = "3,9,7,9,10,9,4,9,99,-1,8\n",
            .end_state = { 3, 9, 7, 9, 10, 9, 4, 9, 99, 0, 8 },
            .input = { 10 },
            .output = { 0 }
        },

        // test if input is equal to 8, using immediate mode
        intcode_test {
            .program = "3,3,1108,-1,8,3,4,3,99\n",
            .end_state = { 3, 3, 1108, 1, 8, 3, 4, 3, 99 },
            .input = { 8 },
            .output = { 1 }
        },
        intcode_test {
            .program = "3,3,1108,-1,8,3,4,3,99\n",
            .end_state = { 3, 3, 1108, 0, 8, 3, 4, 3, 99 },
            .input = { 10 },
            .output = { 0 }
        },

        // test if input is less than 8, using immediate mode
        intcode_test {
            .program = "3,3,1107,-1,8,3,4,3,99\n",
            .end_state = { 3, 3, 1107, 1, 8, 3, 4, 3, 99 },
            .input = { 6 },
            .output = { 1 }
        },
        intcode_test {
            .program = "3,3,1107,-1,8,3,4,3,99\n",
            .end_state = { 3, 3, 1107, 0, 8, 3, 4, 3, 99 },
            .input = { 10 },
            .output = { 0 }
        },

        // test jumps, output 0 if input is zero, 1 if non-zero.
        intcode_test { // position mode
            .program = "3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9\n",
            .end_state = { 3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, 10, 1, 1, 9 },
            .input = { 10 },
            .output = { 1 }
        },
        intcode_test { // position mode
            .program = "3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9\n",
            .end_state = { 3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, 0, 0, 1, 9 },
            .input = { 0 },
            .output = { 0 }
        },
        intcode_test { // immediate mode
            .program = "3,3,1105,-1,9,1101,0,0,12,4,12,99,1\n",
            .end_state = { 3, 3, 1105, 10, 9, 1101, 0, 0, 12, 4, 12, 99, 1 },
            .input = { 10 },
            .output = { 1 }
        },
        intcode_test { // immediate mode
            .program = "3,3,1105,-1,9,1101,0,0,12,4,12,99,1\n",
            .end_state = { 3, 3, 1105, 0, 9, 1101, 0, 0, 12, 4, 12, 99, 0 },
            .input = { 0 },
            .output = { 0 }
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
            case JUMP_EQ: return "JUMP_EQ";
            case JUMP_NEQ: return "JUMP_NEQ";
            case CMP_LESS: return "CMP_LESS";
            case CMP_EQ: return "CMP_EQ";
            case HALT: return "HALT";
            case OFFSET_RBP: return "OFFSET_RBP";
        }

        assert(false);
        return "<ERR>";
    }
}

void intcode_solver_t::run_tests()
{
    for (const auto& test: test_data)
    {
        intcode_solver_t solver(test.program);

        solver.io.set_input(test.input);
        solver.execute();
        const auto& output = solver.io.get_output();

        if (solver.memory != test.end_state)
        {
            fmt::print("Unexpected end state\n");
            fmt::print("    - program: {}\n", test.program);
            fmt::print("    - expected: "); print_row(test.end_state);
            fmt::print("    - actual  : "); print_row(solver.program);
        }
        if (output != test.output)
        {
            fmt::print("Unexpected output\n");
            fmt::print("    - program: {}\n", test.program);
            fmt::print("    - expected: "); print_row(test.output);
            fmt::print("    - actual  : "); print_row(output);
        }
        assert(solver.memory == test.end_state);
        assert(output == test.output);
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

    fmt::print("  output     : "); print_row(io.get_output()); fmt::print("\n");

    fmt::print("\n");
    std::this_thread::sleep_for(100ms);
}
