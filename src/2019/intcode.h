#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <helpers/string.h>

enum class instr: int32_t
{
    SUM = 1,
    MULTIPLY = 2,
    HALT = 99
};

struct intcode_solver_t
{
  public:
    intcode_solver_t()
    {
    }

    intcode_solver_t(const sv& input)
        : program(to_multi_int<int32_t>(input))
    {
    }

    intcode_solver_t(const std::string& input)
        : intcode_solver_t(sv { input.data(), input.length() })
    {
    }

    void execute()
    {
        while (true)
        {
            switch (static_cast<instr>(program[ip]))
            {
            case instr::SUM:
                program[program[ip + 3]] = program[program[ip + 1]] + program[program[ip + 2]];
                break;
            case instr::MULTIPLY:
                program[program[ip + 3]] = program[program[ip + 1]] * program[program[ip + 2]];
                break;
            case instr::HALT:
                return;
            }

            ip += 4;
        }
    }

    static void run_tests();

  public:
    size_t ip = 0; // instruction pointer
    std::vector<int32_t> program;
};
