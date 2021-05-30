#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <helpers/string.h>

enum instr: uint8_t
{
    SUM         = 1, // arg1 + arg2 => arg3
    MULTIPLY    = 2, // arg1 * arg2 => arg3
    INPUT       = 3, // write input to arg1
    OUTPUT      = 4, // output value of arg1

    JUMP_EQ     = 5,
    JUMP_NEQ    = 6,
    CMP_LESS    = 7,
    CMP_EQ      = 8,

    HALT        = 99
};

class intcode_solver_io_t
{
  public:
    void set_input(const std::vector<int32_t>& data)
    {
        input_data = data;
    }

    void append_input(int32_t value)
    {
        input_data.push_back(value);
    }

    std::vector<int32_t> get_output()
    {
        return output_data;
    }

    void reset()
    {
        idp = 0;
        input_data.clear();
        output_data.clear();
    }

  private:
    friend class intcode_solver_t;
    int32_t get_next_input()
    {
        return input_data[idp++];
    }

    void append_output(int32_t value)
    {
        output_data.push_back(value);
    }

  public:
    size_t idp = 0; // input data pointer
    std::vector<int32_t> input_data;
    std::vector<int32_t> output_data;
};

class intcode_solver_t
{
  public:
    intcode_solver_t()
    {
        reset();
    }

    intcode_solver_t(const sv& input)
        : program(to_multi_int<int32_t>(input))
    {
        reset();
    }

    intcode_solver_t(const std::string& input)
        : intcode_solver_t(sv { input.data(), input.length() })
    {
        reset();
    }

    void execute(bool accept_yields = false)
    {
        while (!is_halted)
        {
            debug_pre_exec();

            bool yield = execute_once();

            debug_post_exec();

            if (yield && accept_yields)
                break;
        }
    }

    void reset()
    {
        ip = 0;
        is_halted = false;
        memory = program;
        io.reset();
    }

    static void run_tests();

  private:
    // `n` -> position argument from instruction pointer
    // `immediate` -> parameter mode (true: value as is, false: memory addr)
    int32_t get_param(uint8_t n, bool immediate) const
    {
        if (immediate)
            return get_addr(n);
        else
            return memory.at(get_addr(n));
    }

    int32_t get_addr(uint8_t n) const
    {
        return memory.at(ip + n);
    }

    // Get {opcode, mode of 1st param, mode of 2nd param, mode of 3rd param }
    std::tuple<uint8_t, bool, bool, bool> get_opcode() const
    {
        const auto& digits = get_digits<5>(memory[ip]);
        return
        {
            (digits[3] * 10 + digits[4]),
            digits[2] == 1,
            digits[1] == 1,
            digits[0] == 1,
        };
    }

    // returns true if yielding requested, false if can continue
    bool execute_once()
    {
        const auto& [opcode, m1, m2, m3] = get_opcode();
        switch (opcode)
        {
            case instr::SUM:
            {
                memory[get_addr(3)] = get_param(1, m1) + get_param(2, m2);
                ip += 4;
                break;
            }
            case instr::MULTIPLY:
            {
                memory[get_addr(3)] = get_param(1, m1) * get_param(2, m2);
                ip += 4;
                break;
            }
            case instr::INPUT:
            {
                memory[get_addr(1)] = io.get_next_input();
                ip += 2;
                break;
            }
            case instr::OUTPUT:
            {
                io.append_output(get_param(1, m1));
                ip += 2;
                break;
            }
            case instr::JUMP_EQ:
            {
                if (get_param(1, m1) != 0)
                    ip = get_param(2, m2);
                else
                    ip += 3;
                break;
            }
            case instr::JUMP_NEQ:
            {
                if (get_param(1, m1) == 0)
                    ip = get_param(2, m2);
                else
                    ip += 3;
                break;
            }
            case instr::CMP_LESS:
            {
                memory[get_addr(3)] = (get_param(1, m1) < get_param(2, m2)) ? 1 : 0;
                ip += 4;
                break;
            }
            case instr::CMP_EQ:
            {
                memory[get_addr(3)] = (get_param(1, m1) == get_param(2, m2)) ? 1 : 0;
                ip += 4;
                break;
            }
            case instr::HALT:
            {
                is_halted = true;
                break;
            }
        }

        return (opcode == instr::OUTPUT);
    }

    void debug_pre_exec();
    void debug_post_exec();

  public:
    size_t ip = 0; // instruction pointer

    bool is_halted = false;

    const std::vector<int32_t> program; // program, constant (always equal to input)
    std::vector<int32_t> memory; // program, as loaded into memory, may be modified

    intcode_solver_io_t io;
};
